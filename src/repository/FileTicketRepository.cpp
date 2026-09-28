#include "repository/FileTicketRepository.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "models/Ticket.h"
#include "utils/TextUtils.h"

FileTicketRepository::FileTicketRepository(std::filesystem::path outputDir)
    : outputDir_(std::move(outputDir)) {
    std::filesystem::create_directories(outputDir_);
}

void FileTicketRepository::save(const Ticket& ticket) {
    const std::filesystem::path path = fileFor(ticket.category());
    std::ofstream out(path, std::ios::app);
    if (!out) throw std::runtime_error("Could not open " + path.string());

    std::vector<std::string> details;
    for (const auto& [key, value] : ticket.details()) details.push_back(key + "=" + value);

    const std::string separator(60, '=');
    out << separator << '\n'
        << "Ticket ID  : " << ticket.id() << '\n'
        << "Created    : " << ticket.createdAt() << '\n'
        << "Priority   : " << toString(ticket.priority()) << '\n'
        << "Category   : " << toString(ticket.category()) << '\n'
        << "Sentiment  : " << toString(ticket.sentiment().label) << " (" << ticket.sentiment().score << ")\n"
        << "State      : " << ticket.stateName() << '\n'
        << "Lifecycle  : " << text::join(ticket.stateHistory(), " -> ") << '\n'
        << "Details    : " << (details.empty() ? "-" : text::join(details, ", ")) << '\n'
        << "Subject    : " << ticket.subject() << '\n'
        << "Description: " << ticket.description() << '\n'
        << "Reply      :\n";

    std::istringstream replyLines(ticket.reply());
    std::string line;
    while (std::getline(replyLines, line)) out << "    " << line << '\n';
    out << separator << "\n\n";
}

std::string FileTicketRepository::loadAll(Category category) const {
    std::ifstream in(fileFor(category));
    if (!in) return "";
    std::ostringstream content;
    content << in.rdbuf();
    return content.str();
}

std::filesystem::path FileTicketRepository::fileFor(Category category) const {
    return outputDir_ / (text::toLower(toString(category)) + ".txt");
}
