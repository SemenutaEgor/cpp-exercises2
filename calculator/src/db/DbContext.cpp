#include "DbContext.h"

DbContext::DbContext(std::string conninfo) : conn_(conninfo), storage_(conn_) {}