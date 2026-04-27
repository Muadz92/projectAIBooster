#ifndef BOOSTER_ENGINE_HPP
#define BOOSTER_ENGINE_HPP

#include <string>

class BoosterEngine {
public:
    std::string analyzeSystem(double load, double ram) const {
        if (ram < 0.5) {
            return "🛑 CRITICAL: CLOSE APPS";
        } else if (load > 1.2) {
            return "⚠️ HEAVY LOAD: COOLING";
        }
        return "✅ SYSTEM OPTIMAL";
    }
};

#endif