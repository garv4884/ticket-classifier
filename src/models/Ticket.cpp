#include "models/Ticket.h"

#include "states/NewState.h"
#include "utils/TextUtils.h"

Ticket::Ticket(std::string id, std::string subject, std::string description,
               Priority priority, SentimentResult sentiment)
    : id_(std::move(id)),
      subject_(std::move(subject)),
      description_(std::move(description)),
      priority_(priority),
      sentiment_(sentiment),
      createdAt_(text::currentTimestamp()),
      state_(std::make_unique<NewState>()) {
    stateHistory_.push_back(state_->name());
}

Ticket::~Ticket() = default;

void Ticket::startProcessing() { state_->startProcessing(*this); }
void Ticket::startAnswering() { state_->startAnswering(*this); }
void Ticket::markReplied() { state_->markReplied(*this); }
void Ticket::close() { state_->close(*this); }
void Ticket::escalate() { state_->escalate(*this); }

void Ticket::changeState(std::unique_ptr<TicketState> next) {
    state_ = std::move(next);
    stateHistory_.push_back(state_->name());
}

std::string Ticket::stateName() const { return state_->name(); }

std::optional<std::string> Ticket::detail(const std::string& key) const {
    const auto it = details_.find(key);
    if (it == details_.end()) return std::nullopt;
    return it->second;
}
