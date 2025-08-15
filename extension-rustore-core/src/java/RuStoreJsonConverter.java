package ru.rustore.defold.core;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

import org.json.JSONArray;
import org.json.JSONObject;
import android.util.Log;

public class RuStoreJsonConverter {

    public static final String TAG = "RuStoreJsonConverter";

	public static String convertIdent(String productId) {
		// String ident = productId.replace("com.happygames.ranch", "com.happygames.mergecafe");
		// Log.d(TAG, "convertProductDetails() ident: " + ident);
		// return ident;
		return productId;
	}

    public static String convertProductDetails(String jsonString) {
        try {

			JSONArray jsonArray = new JSONArray(jsonString);
			JSONArray transformedArray = new JSONArray();
			
			for (int i = 0; i < jsonArray.length(); i++) {
				JSONObject original = jsonArray.getJSONObject(i);

				String ident = convertIdent(original.getString("productId"));
				Log.d(TAG, "INFO:RUSTORECORE: convertProductDetails() ident: " + ident);
            	original.put("ident", ident);
				original.put("currency_code", original.get("currency"));
				original.put("price_string", original.get("priceLabel"));

				if (original.has("price")) {
					original.put("price", original.getDouble("price")*.01);
				}

				transformedArray.put(original);
			}

			return transformedArray.toString();

        } catch (Exception e) {
            e.printStackTrace();
            return "[]";
        }
    }

    private static String toISO8601(final Date date) {
        String formatted = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ssZ").format(date);
        return formatted.substring(0, 22) + ":" + formatted.substring(22);
    }

	// enum TransactionState
	// {
	// 	TRANS_STATE_PURCHASING = 0,
	// 	TRANS_STATE_PURCHASED = 1,
	// 	TRANS_STATE_FAILED = 2,
	// 	TRANS_STATE_RESTORED = 3,
	// 	TRANS_STATE_UNVERIFIED = 4,
	// };

	private static int purchaseStateToDefoldState(String purchaseType) {
        int defoldState = 4;
        switch(purchaseType) {
            // case PurchaseState.PENDING:
            //     defoldState = IapJNI.TRANS_STATE_PURCHASING;
            //     break;
			case "Failure":
			case "Cancelled":
                defoldState = 2;
                break;
            case "PAID":
            case "Success":
                defoldState = 1;
                break;
            default:
            // case PurchaseState.UNSPECIFIED_STATE:
                defoldState = 4;
                break;
        }
        return defoldState;
    }

	public static String convertPurchasesDetails(String jsonString) {

		// [
		// 	{
		// 		"amount": 7900,
		// 		"amountLabel": "79.00 руб.",
		// 		"currency": "RUB",
		// 		"developerPayload": "",
		// 		"invoiceId": "1000075467",
		// 		"language": "ru-RU",
		// 		"orderId": "f403e6ae-4c40-41c8-8093-2b6bb44fb55c",
		// 		"productId": "com.happygames.ranch.sku00199",
		// 		"productType": "CONSUMABLE",
		// 		"purchaseId": "4a2e0c11-d2a0-4e60-aa04-c84eed8da50e",
		// 		"purchaseState": "PAID",
		// 		"purchaseTime": "Jul 22, 2025 7:25:31 PM",
		// 		"quantity": 1,
		// 		"subscriptionToken": "1000075467.2521001636"
		// 	}
		// ]

        try {

			JSONArray jsonArray = new JSONArray(jsonString);
			
			for (int i = 0; i < jsonArray.length(); i++) {
				JSONObject original = jsonArray.getJSONObject(i);

				String purchaseState = original.getString("purchaseState");
				if(purchaseState.equals("INVOICE_CREATED")){
					continue;
				}

				String ident = convertIdent(original.getString("productId"));
				Log.d(TAG, "INFO:RUSTORECORE: convertProductDetails() ident: " + ident);

            	original.put("ident", ident);
				original.put("currency_code", original.get("currency"));
				original.put("price_string", original.get("amountLabel"));
				original.put("state", purchaseStateToDefoldState(purchaseState));
				original.put("trans_ident", original.get("orderId"));
				original.put("date", toISO8601(new Date()));
				original.put("receipt", original.get("purchaseId"));
				original.put("signature", original.get("subscriptionToken"));
				original.put("original_json", "{}");

				if (original.has("amount")) {
					original.put("amount", original.getDouble("amount")*.01);
				}

				return original.toString();
			}

			return "{ \"state\":2, \"date\":\"" + toISO8601(new Date()) + "\", \"ident\":\"\" }";

        } catch (Exception e) {
            e.printStackTrace();
            return "{}";
        }
    }


	public static String getPurchaseProductFailure() {
		try {
			JSONObject original = new JSONObject();
			original.put("ident", "");
			original.put("state", 2);
			original.put("date", toISO8601(new Date()));
			original.put("trans_ident", "");
			String result = original.toString();
			Log.d(TAG, "INFO:RUSTORECORE: getPurchaseProductFailure() => " + result);
			return result;
        } catch (Exception e) {
            e.printStackTrace();
            return "{}";
        }
	}

	public static String convertPurchaseProductFailure(String jsonString) {
		//{"productId": "com.happygames.mergecafe.sku00199", "cause": {"detailMessage":"RuStore User Not Authorized","simpleName":"RuStoreUserUnauthorizedException"}}
		try {

			JSONObject original = new JSONObject(jsonString);
			String ident = "";
			if(original.has("productId")){
				ident = convertIdent(original.getString("productId"));
			}

			original.put("ident", ident);
			original.put("state", 2);
			original.put("date", toISO8601(new Date()));
			original.put("trans_ident", "");
			String result = original.toString();
			Log.d(TAG, "INFO:RUSTORECORE: convertPurchaseProductFailure() => " + result);
			return result;

        } catch (Exception e) {
            e.printStackTrace();
            return "{}";
        }
	}

	public static String convertPurchaseDetails(String jsonString) {

		// {
		// 	"type": "Success",
		// 	"data": {
		// 		"invoiceId": "1000075467",
		// 		"orderId": "f403e6ae-4c40-41c8-8093-2b6bb44fb55c",
		// 		"productId": "com.happygames.ranch.sku00199",
		// 		"purchaseId": "4a2e0c11-d2a0-4e60-aa04-c84eed8da50e",
		// 		"sandbox": true,
		// 		"subscriptionToken": "1000075467.2521001636"
		// 	}
		// }

		//{"type":"Failure","data":{"orderId":"63917391-e14a-4ad8-a3df-d3db8ae9bf2e","productId":"com.happygames.mergecafe.sku00199","quantity":1,"sandbox":true}}

        try {

			JSONObject original = new JSONObject(jsonString);
			JSONObject data = original.getJSONObject("data");

			String type = original.getString("type");
			String ident = "";
			if(data.has("productId")){
				ident = convertIdent(data.getString("productId"));
			}

			original.put("ident", ident);
			original.put("state", purchaseStateToDefoldState(type));
			original.put("date", toISO8601(new Date()));
			if(data.has("orderId")){
				original.put("trans_ident", data.get("orderId"));
			}

			switch(type) {
				case "Failure":
					break;
				case "Success":
					if(data.has("purchaseId")){
						original.put("receipt", data.get("purchaseId"));
					}
					if(data.has("subscriptionToken")){
						original.put("signature", data.get("subscriptionToken"));
					}
					original.put("original_json", jsonString);
					break;
        	}
			String result = original.toString();
			Log.d(TAG, "INFO:RUSTORECORE: convertPurchaseDetails() => " + result);
			return result;

        } catch (Exception e) {
            e.printStackTrace();
            return "{}";
        }
    }
}
