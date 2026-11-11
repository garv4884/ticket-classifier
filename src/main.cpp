// Composition root: builds every concrete object once and wires them together
// through constructors. Nothing below main() creates its own dependencies.

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

#include "controllers/ConsoleController.h"
#include "controllers/DemoController.h"
#include "controllers/FileController.h"
#include "factories/TicketFactory.h"
#include "repliers/ReplierFactory.h"
#include "repository/FileIdempotencyStore.h"
#include "repository/FileTicketRepository.h"
#include "services/TicketService.h"
#include "services/classification/KeywordClassifier.h"
#include "services/sentiment/LexiconSentimentAnalyzer.h"
#include "utils/IdGenerator.h"

namespace {

enum class Mode { CONSOLE, DEMO, FILE };

struct Options {
    Mode mode = Mode::CONSOLE;
    std::filesystem::path inputFile;
    std::filesystem::path dataDir = "data/output";
    bool reset = false;
};

void printUsage() {
    std::cout << "Usage: ticketing [--demo | --file <path>] [--data <dir>] [--reset]\n"
                 "  (no flag)      interactive console\n"
                 "  --demo         run the built-in showcase tickets\n"
                 "  --file <path>  process tickets from a text file\n"
                 "  --data <dir>   output folder (default: data/output)\n"
                 "  --reset        delete stored tickets and the idempotency index first\n";
}

bool parseArgs(int argc, char* argv[], Options& options) {
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--demo") {
            options.mode = Mode::DEMO;
        } else if (arg == "--file" && i + 1 < argc) {
            options.mode = Mode::FILE;
            options.inputFile = argv[++i];
        } else if (arg == "--data" && i + 1 < argc) {
            options.dataDir = argv[++i];
        } else if (arg == "--reset") {
            options.reset = true;
        } else {
            return false;
        }
    }
    return true;
}

}  // namespace

int main(int argc, char* argv[]) {
    Options options;
    if (!parseArgs(argc, argv, options)) {
        printUsage();
        return 1;
    }

    try {
        // Prevent arbitrary directory deletion or escaping the application root.
        const std::filesystem::path canonicalDataDir =
            std::filesystem::weakly_canonical(options.dataDir);
        const std::filesystem::path currentPath = std::filesystem::current_path();

        if (options.reset) {
            // Refuse to reset root or directories outside the current working directory.
            auto rel = std::filesystem::relative(canonicalDataDir, currentPath);
            if (rel.empty() || rel.string().find("..") != std::string::npos || rel == ".") {
                std::cerr << "Refusing to reset unsafe directory: " << canonicalDataDir << '\n';
                return 1;
            }
            std::filesystem::remove_all(options.dataDir);
        }

        KeywordClassifier classifier;
        LexiconSentimentAnalyzer sentimentAnalyzer;
        TicketFactory ticketFactory;
        ReplierFactory replierFactory;
        FileTicketRepository repository(options.dataDir);
        FileIdempotencyStore idempotencyStore(options.dataDir / "idempotency_index.txt");
        // Every stored ticket has one index entry, so numbering continues after them.
        IdGenerator idGenerator(static_cast<int>(idempotencyStore.size()) + 1);

        TicketService service(classifier, sentimentAnalyzer, ticketFactory, replierFactory,
                              repository, idempotencyStore, idGenerator);

        std::unique_ptr<ITicketController> controller;
        switch (options.mode) {
            case Mode::DEMO:
                controller = std::make_unique<DemoController>(service);
                break;
            case Mode::FILE:
                controller = std::make_unique<FileController>(service, options.inputFile);
                break;
            case Mode::CONSOLE:
                controller = std::make_unique<ConsoleController>(service, repository);
                break;
        }
        controller->run();

        std::cout << "\nTickets are stored in " << std::filesystem::absolute(options.dataDir).string() << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Fatal error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
