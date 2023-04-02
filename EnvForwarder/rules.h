#pragma once
#include <list>
#include <map>
#include <string>

namespace my
{
    enum RuleType
    {
        RULE_INVALID,
        RULE_PREPEND,
        RULE_APPEND,
        RULE_REMOVE
    };

    struct rule
    {
        RuleType type;
        std::string expression;
    };

    typedef std::list<rule> rules;
    typedef std::map<std::string, rules> envrules;

    class EnvironmentRules
    {
    public:
        EnvironmentRules();
        EnvironmentRules(const std::string& path);
	    bool open(const std::string& path);
        bool isOk() const;
        bool apply();

    private:
        void applyRules(std::string& value, const rules& rules);

        envrules m_envRules;
        bool m_ok;
    };
}