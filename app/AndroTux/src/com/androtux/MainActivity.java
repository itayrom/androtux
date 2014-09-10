/*
**  Android Remote for Tux (AndroTux) 
**  Copyright (C) 2014 Itay Rom <1t4yr0m@gmail.com> Tal Sabo <tal.sabo@gmail.com> 
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

package com.androtux;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends FragmentActivity implements SensorEventListener {
	private static boolean _axis;
	private float _maxRange, _multiplier;
	
	private SensorManager _sManager;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        CommunicationHandler cHandler = CommunicationHandler.getInstance();
        cHandler.setActivity(this);
        cHandler.initialize();
		
        _sManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        
        _maxRange = _sManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER).getMaximumRange();
        _multiplier = (1024 / _maxRange) * 4;
        
        _axis = false;
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
    @Override
    protected void onResume() {  
        super.onResume();  
        CommunicationHandler.getInstance().connect();
        _sManager.registerListener(this, _sManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),SensorManager.SENSOR_DELAY_FASTEST);  
    }  
  
    @Override  
    protected void onStop() { 
    	super.onStop();
        _sManager.unregisterListener(this);
        CommunicationHandler.getInstance().disconnect();
    }
    
    @Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		
	}
    
    @Override  
    public void onSensorChanged(SensorEvent event) { 
    	if (_axis) {
    		float x = event.values[1] *_multiplier;
    		float y = (event.values[0] - _maxRange / 3) *_multiplier;
    		String str = "a" + x + "," + y;
    		CommunicationHandler.getInstance().sendData(str);
    	}
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
        	ToggleButton tbSettings = (ToggleButton)findViewById(R.id.tbSettings);
        	tbSettings.setOn();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
    
    @Override
	protected void onPause() {
    	super.onPause();
		CommunicationHandler.getInstance().disconnect();
	}
    
	@Override
	protected void onDestroy() {
		super.onDestroy();
		CommunicationHandler.getInstance().disconnect();
	}
	
	public static void setAxis(boolean stat) {
		_axis = stat;
	}
}
