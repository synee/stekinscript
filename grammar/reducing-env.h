#ifndef __STEKIN_GRAMMAR_REDUCING_ENVIRONMENT_H__
#define __STEKIN_GRAMMAR_REDUCING_ENVIRONMENT_H__

#include <string>
#include <vector>

namespace grammar {

    struct ArgReducingEnv {
        ArgReducingEnv()
            : _async_index(-1)
        {}

        void setAsync(misc::position const& pos, int index, std::vector<std::string> const& ap);
        bool isAsync() const;
        int asyncIndex() const;
        std::vector<std::string> const& asyncParams() const;
    private:
        int _async_index;
        std::vector<std::string> _async_params;
    };

}

#endif /* __STEKIN_GRAMMAR_REDUCING_ENVIRONMENT_H__ */
