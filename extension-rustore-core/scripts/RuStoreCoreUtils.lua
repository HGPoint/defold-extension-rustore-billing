-- To get access to the functions, you need to put:
-- require "extension_rustore_core.scripts.RuStoreCoreUtils"
-- in any script using the functions.

local RuStoreCoreUtils = {}

_rustore_core_utils_instance = nil

function RuStoreCoreUtils:new()
	local obj = {}
	setmetatable(obj, self)
	self.__index = self
	return obj
end


function RuStoreCoreUtils:get_instance()
	if not _rustore_core_utils_instance then
		_rustore_core_utils_instance = self:new()
	end
	return _rustore_core_utils_instance
end


function RuStoreCoreUtils:init()
	rustorecore.init()
end


function RuStoreCoreUtils:connect(channel, callback)

end


function RuStoreCoreUtils:log_warning(tag, message)
	rustorecore.log_warning(tag, message)
end

return RuStoreCoreUtils
