package ru.rustore.defold.billing;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import java.util.UUID;

public class RuStoreIntentFilterActivity extends Activity {

	private final String defoldActivityClassName = "com.dynamo.android.DefoldActivity";

	private Class<?> getActivityClass(String activityClassName) {
		Class<?> activityClass = null;
		try {
			activityClass = Class.forName(activityClassName);
		} catch(ClassNotFoundException ex) {
		}

		return activityClass;
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		if (savedInstanceState == null) {
			RuStoreBilling.onNewIntent(getIntent());
		}

		startGameActivity(null, defoldActivityClassName);
		finish();
	}

	@Override
	public void onNewIntent(Intent newIntent)
	{
		super.onNewIntent(newIntent);

		RuStoreBilling.onNewIntent(newIntent);
		startGameActivity(newIntent, defoldActivityClassName);
		finish();
	}

	private void startGameActivity(Intent intent, String gameActivityClassName) {
		Class<?> gameActivityClass = getActivityClass(gameActivityClassName);
		if (gameActivityClass != null) {
			Intent newIntent = new Intent(this, gameActivityClass);
			if (intent != null) newIntent.putExtras(intent.getExtras());
			startActivity(newIntent);
		}
	}

    public static String getUUID() {
        return UUID.randomUUID().toString();
    }
}
