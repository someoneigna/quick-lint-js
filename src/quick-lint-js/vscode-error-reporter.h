// Copyright (C) 2020  Matthew Glazar
// See end of file for extended copyright information.

#ifndef QUICK_LINT_JS_VSCODE_ERROR_REPORTER_H
#define QUICK_LINT_JS_VSCODE_ERROR_REPORTER_H

#include <cstdint>
#include <quick-lint-js/char8.h>
#include <quick-lint-js/error-formatter.h>
#include <quick-lint-js/error.h>
#include <quick-lint-js/lsp-location.h>
#include <quick-lint-js/monotonic-allocator.h>
#include <quick-lint-js/padded-string.h>
#include <quick-lint-js/token.h>
#include <vector>

struct qljs_vscode_diagnostic;

namespace quick_lint_js {
class vscode_error_formatter;

class vscode_error_reporter final : public error_reporter {
 public:
  explicit vscode_error_reporter();

  void set_input(padded_string_view input, const lsp_locator *);
  void reset();

  const qljs_vscode_diagnostic *get_diagnostics();

#define QLJS_ERROR_TYPE(name, code, struct_body, format) \
  void report(name) override;
  QLJS_X_ERROR_TYPES
#undef QLJS_ERROR_TYPE

 private:
  vscode_error_formatter format(const char *code);

  char8 *allocate_c_string(string8_view);

  std::vector<qljs_vscode_diagnostic> diagnostics_;
  const lsp_locator *locator_;
  const char8 *input_;
  monotonic_allocator string_allocator_;

  friend vscode_error_formatter;
};

class vscode_error_formatter : public error_formatter<vscode_error_formatter> {
 public:
  explicit vscode_error_formatter(vscode_error_reporter *reporter,
                                  const char *code);

  void write_before_message(severity, const source_code_span &origin);
  void write_message_part(severity, string8_view);
  void write_after_message(severity, const source_code_span &origin);

 private:
  vscode_error_reporter *reporter_;
  const char *code_;
  string8 current_message_;
};
}

#endif

// quick-lint-js finds bugs in JavaScript programs.
// Copyright (C) 2020  Matthew Glazar
//
// This file is part of quick-lint-js.
//
// quick-lint-js is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// quick-lint-js is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with quick-lint-js.  If not, see <https://www.gnu.org/licenses/>.
