#include "rules.h"
#include "logger.h"
#include <regex>
#include "system.h"
#include <fstream>

namespace fs = std::filesystem;
namespace sys = my::system;

namespace my
{
EnvironmentRules::EnvironmentRules()
{
    m_ok = false;
}
EnvironmentRules::EnvironmentRules(const std::string& path)
{
    m_ok = open(path);
}

bool EnvironmentRules::isOk() const
{
    return m_ok;
}

bool EnvironmentRules::open(const std::string& path)
{
    m_envRules.clear();
    m_ok = true;

    std::ifstream fProgram(path, std::ios::in);
    if (!fProgram.is_open()) {
        Log().Get(LOG_WARNING) << "Could not open rule file. Make sure you created a rules.txt file containing your rules!";
        m_ok = false;
        return false;
    }

    for (std::string line; std::getline(fProgram, line);) {
        if (line.empty()) {
            continue;
        }
        bool ruleOk = true;
        rule rule;
        std::string tokenVariable;
        std::string tokenExpression;

        size_t idxSeparatorVariable = line.find(' ');
        if (idxSeparatorVariable != std::string::npos) {
            size_t idxSeparatorRule = line.find(' ', idxSeparatorVariable + 1);

            tokenVariable = line.substr(0, idxSeparatorVariable);
            std::string tokenRule = line.substr(idxSeparatorVariable + 1, idxSeparatorRule - idxSeparatorVariable - 1);
            if (idxSeparatorRule != std::string::npos)
            {
                tokenExpression = line.substr(idxSeparatorRule + 1, -1);
            }

            if (tokenRule == "+" || tokenRule == "ADD")
            {
                rule.type = RULE_ADD;
            }
            else if (tokenRule == "-" || tokenRule == "REMOVE")
            {
                rule.type = RULE_REMOVE;
            }
            else
            {
                ruleOk = false;
            }
        }
        else {
            ruleOk = false;
        }
        if (!ruleOk) {
            Log().Get(LOG_WARNING) << "Invalid rule \"" << line << "\"";
            m_ok = false;
            continue;
        }
        rule.expression = tokenExpression;
        m_envRules[tokenVariable].push_back(rule);
    }
    return m_ok;
}

void EnvironmentRules::applyRules(std::string& value, const rules& rules)
{
    for (const rule& rule : rules) {
        if (rule.type == RULE_ADD) {
            if (!value.empty() && !value.ends_with(';')) {
                value += ";";
            }
            value += rule.expression;
        }
        else if (rule.type == RULE_REMOVE) {
            if (!value.empty()) {
                std::regex remove(rule.expression);
                value = std::regex_replace(value, remove, "");
            }
        }
    }
}

bool EnvironmentRules::apply()
{
    if (!isOk()) {
        return false;
    }

    bool ok = true;
    for (auto it = m_envRules.begin(); it != m_envRules.end(); ++it) {
        const std::string& env = it->first;
        const rules& rules = it->second;
        std::string value = sys::getEnvironmentVariable(env);
        applyRules(value, rules);
        if (!sys::setEnvironmentVariable(env, value)) {
            ok = false;
        }
    }
    return ok;
}
}