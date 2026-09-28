# Customer Support Ticket Router (C++17 LLD)

Accepts support tickets (subject, description, priority P0-P2), routes them to a
category by keyword scoring, runs sentiment analysis, generates a pre-written
reply tailored to the ticket, and stores it in that category's text file.
Duplicate submissions are detected and answered with the original ticket
(idempotency).

## Build and run

```bash
cmake -S . -B build -G "MinGW Makefiles"   # or any generator
cmake --build build

# run from the project root
bin/ticketing --demo --reset                    # showcase: all categories, sentiments, duplicates
bin/ticketing --file data/input/sample_tickets.txt
bin/ticketing                                   # interactive console
bin/ticketing_tests                             # unit tests
```

Options: `--data <dir>` changes the output folder (default `data/output`),
`--reset` clears stored tickets and the idempotency index first.

## Flow

```
Controller (Console / File / Demo)
   -> TicketService.submit(TicketRequest)
        validate -> idempotency check (duplicate? return original)
        -> KeywordClassifier (category) + LexiconSentimentAnalyzer (sentiment)
        -> TicketFactory creates BillingTicket / DeliveryTicket / ...        [NEW]
        -> extract details (order id, invoice, email...), escalation check   [PROCESSING]
        -> ReplierFactory -> category replier builds the reply               [ANSWER_GENERATING]
        -> [REPLIED] -> [CLOSED]  or  [ESCALATED] (angry + P0)
        -> FileTicketRepository writes data/output/<category>.txt
        -> FileIdempotencyStore records the key
```

## Structure

| Folder | Contents |
|---|---|
| `src/controllers` | `ITicketController`; `ConsoleController`, `FileController`, `DemoController`; `ResultPrinter` |
| `src/services` | `TicketService` (orchestrator); `classification/` and `sentiment/` strategies |
| `src/models` | enums, `TicketRequest`, `SubmissionResult`, `TicketRecord`, abstract `Ticket` + `tickets/` subclasses |
| `src/states` | `TicketState` and the six lifecycle states |
| `src/repliers` | `IReplier`, `BaseReplier` (template method), one replier per category, `ReplierFactory` |
| `src/factories` | `TicketFactory` |
| `src/repository` | `ITicketRepository` / `FileTicketRepository`, `IIdempotencyStore` / `FileIdempotencyStore` |
| `src/utils` | text helpers, `Fingerprint` (idempotency key), `IdGenerator` |

## Design patterns

- **Strategy** - `IClassifier`, `ISentimentAnalyzer`, `IReplier`: swap keyword
  matching or the lexicon for something smarter without touching the service.
- **Factory** - `TicketFactory` (category -> ticket subclass), `ReplierFactory`
  (category -> replier). These are the only places that switch on category.
- **State** - `TicketState` subclasses allow only legal transitions; e.g.
  closing a `NEW` ticket throws.
- **Template Method** - `BaseReplier::generate()` fixes the reply skeleton
  (empathy line by sentiment, body by category sub-topic, SLA line by
  priority, sign-off); subclasses provide only `body()`.

Dependencies are created once in `main.cpp` and injected through constructors;
`TicketService` sees only interfaces, which is also how the tests swap in
in-memory repositories.

## Idempotency

The key is a FNV-1a 64-bit hash of the normalized subject + description
(lowercased, punctuation and extra spaces removed), so
`"PACKAGE NOT DELIVERED!!"` and `"package not delivered"` are the same ticket.
Priority is not part of the key. Keys are persisted in
`data/output/idempotency_index.txt`, so duplicates are caught across runs; a
duplicate creates nothing and returns the original ticket id, state and reply.

## Adding a category

1. Add the enum value in `models/Enums.h/.cpp`.
2. Add a `Ticket` subclass and a `BaseReplier` subclass.
3. Register them in `TicketFactory` and `ReplierFactory`.
4. Add its keywords in `KeywordClassifier`.
