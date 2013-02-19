#include <sstream>

#include <util/string.h>

#include "list-pipe.h"

using namespace output;

std::string AsyncPipeResult::str() const
{
    return "$result";
}

static std::string const ASYNC_MAPPER_CONTINUE(
"return $next($index + 1, $result);\n"
);

static std::string const SYNC_PIPELINE_RETURN(
"return [1, #EXPRESSION];\n"
);

void PipelineReturn::write(std::ostream& os) const
{
    os << util::replace_all(
            SYNC_PIPELINE_RETURN
                , "#EXPRESSION", expr->str());
}

void PipelineContinue::write(std::ostream& os) const
{
    os << ASYNC_MAPPER_CONTINUE;
}

static std::string const ASYNC_PIPE(
"(function ($list) {\n"
"    if (!($list) || $list.length === undefined) throw 'not iterable';\n"
"    function $next($index, $result) {\n"
"        var $key = null;\n"
"        if ($index === $list.length) {\n"
"            #SUCCESSIVE_STATEMENTS\n"
"        } else {\n"
"            var $element = $list[$index];\n"
"            #NEXT\n"
"        }\n"
"    }\n"
"    $next(0, []);\n"
"})(#LIST)\n"
);

std::string AsyncPipeline::str() const
{
    std::ostringstream suc_os;
    succession->write(suc_os);
    std::ostringstream rec_os;
    recursion->write(rec_os);
    return std::move(
        util::replace_all(
        util::replace_all(
        util::replace_all(
            ASYNC_PIPE
                , "#SUCCESSIVE_STATEMENTS", suc_os.str())
                , "#NEXT", rec_os.str())
                , "#LIST", list->str())
        );
}

static std::string const SYNC_PIPE(
"(function ($list) {\n"
"    if (!($list)) return;\n"
"    var $result = [];\n"
"    var $ind = 0;\n"
"    var $next = function() {};\n"
"    var $pipeRet;\n"
"    for (var $k in $list) {\n"
"        if ($ind === $list.length) {\n"
"            break;\n"
"        }\n"
"        $pipeRet = (function ($index, $key, $element) {\n"
"           #SECTION\n"
"           return [0];\n"
"        })($ind, $k, $list[$k]);\n"
"        switch ($pipeRet[0]) {\n"
"            case 1: return $pipeRet[1];\n"
"        }\n"
"        ++$ind;\n"
"    }\n"
"    return $result;\n"
"})(#LIST)"
);

std::string SyncPipeline::str() const
{
    std::ostringstream sec_os;
    section->write(sec_os);
    return std::move(
        util::replace_all(
        util::replace_all(
            SYNC_PIPE
                , "#SECTION", sec_os.str())
                , "#LIST", list->str())
        );
}

std::string PipeElement::str() const
{
    return "$element";
}

std::string PipeIndex::str() const
{
    return "$index";
}

std::string PipeKey::str() const
{
    return "$key";
}

std::string PipeResult::str() const
{
    return "$result";
}
