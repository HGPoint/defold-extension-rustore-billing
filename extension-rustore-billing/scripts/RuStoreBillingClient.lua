-- To get access to the functions, you need to put:
-- require "extension_rustore_billing.scripts.RuStoreBillingClient"
-- in any script using the functions.

local RuStoreBillingClient = {}
local log = require("hg.log")
local logger = log.namespace("RUSTORECLIENT")

_rustore_billing_client_instance = nil

function RuStoreBillingClient:new()
	local obj = {}
	setmetatable(obj, self)
	self.__index = self
	return obj
end


function RuStoreBillingClient:get_instance()
	if not _rustore_billing_client_instance then
		_rustore_billing_client_instance = self:new()
	end
	return _rustore_billing_client_instance
end


function RuStoreBillingClient:connect(channel, callback)
	iap.connect(channel, callback)
end


function RuStoreBillingClient:init(app_id, scheme)
	logger.debug("RuStoreBillingClient:init START")
	iap.init(app_id, scheme)
	rustorecore.connect("rustore_on_get_products_success", _on_get_products_success)
    rustorecore.connect("rustore_on_get_products_failure", _on_get_products_failure)
	logger.debug("RuStoreBillingClient:init DONE")
end


function _on_get_products_success(self, channel, value)
	logger.debug("_on_get_products_success:init")
    local data = json.decode(value)
end

function _on_get_products_failure(self, channel, value)
	logger.debug("_on_get_products_failure:init")
    local data = json.decode(value)
end

function RuStoreBillingClient:check_purchases_availability()
	iap.check_purchases_availability()
end


function RuStoreBillingClient:get_products(product_ids)
	iap.get_products(product_ids)
end


function RuStoreBillingClient:purchase_product(product_id)
	iap.purchase_product(product_id)
end


function RuStoreBillingClient:get_purchases()
	iap.get_purchases()
end


function RuStoreBillingClient:confirm_purchase(product_id)
	iap.confirm_purchase(product_id)
end


function RuStoreBillingClient:delete_purchase(product_id)
	iap.delete_purchase(product_id)
end


function RuStoreBillingClient:get_purchase_info(product_id)
	iap.get_purchase_info(product_id)
end


function RuStoreBillingClient:set_theme(product_id)
	iap.set_theme(product_id)
end

return RuStoreBillingClient
