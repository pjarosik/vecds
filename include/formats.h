#ifndef INCLUDE_FORMATS_H
#define INCLUDE_FORMATS_H

#include "lattice.h"

class FormatResult {
public:
    FormatResult() = default;

    FormatResult(const vector<std::string> &warnings, const vector<std::string> &errors)
            : warnings(warnings), errors(errors) {}

    [[nodiscard]] const vector<std::string> &getWarnings() const {
        return warnings;
    }
    [[nodiscard]] const vector<std::string> &getErrors() const {
        return errors;
    }

    bool hasWarnings() const {
        return !warnings.empty();
    }

    bool hasErrors() const {
        return !errors.empty();
    }

    std::string getErrorMessage() const {
        return join(errors);
    }

    std::string getWarningMessage() const {
        return join(warnings);
    }

private:
    std::string join(const std::vector<std::string> &strs) const {
        const char* const d = ", ";
        std::ostringstream output;
        std::copy(strs.begin(), strs.end(), std::ostream_iterator<std::string>(output, d));
        return output.str();
    }

    std::vector<std::string> warnings;
    std::vector<std::string> errors;
};

class FormatResultBuilder {
public:

    void addWarning(std::string warning) {
        warnings.push_back(std::move(warning));
    }

    void addError(std::string error) {
        errors.push_back(std::move(error));
    }

    FormatResult build() {
        return FormatResult(warnings, errors);
    }

private:
    std::vector<std::string> warnings;
    std::vector<std::string> errors;
};

class FormatReadResult {
public:
    FormatReadResult() = default;

    FormatReadResult(const optional<Lattice> &lattice, const FormatResult& res) : lattice(lattice), result(res) {}

    [[nodiscard]] const optional<Lattice> &getLattice() const {
        return lattice;
    }
    [[nodiscard]] const FormatResult &getResult() const {
        return result;
    }

private:
    std::optional<Lattice> lattice;
    FormatResult result;
};

// Legacy format.
FormatReadResult readAlc(QTextStream &in, const QString &name);

FormatReadResult readXyz(QTextStream &in, const QString &name);
FormatResult writeXyz(std::ostream &out, const Lattice &lattice);

FormatReadResult readMol2(QTextStream &in, const QString &name);
FormatResult writeMol2(std::ostream &out, const Lattice &lattice);



#endif //INCLUDE_FORMATS_H
