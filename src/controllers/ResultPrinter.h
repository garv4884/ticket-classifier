#pragma once

#include <ostream>

#include "models/SubmissionResult.h"

// Shared console rendering of a submission result for all controllers.
void printResult(std::ostream& out, const SubmissionResult& result);
