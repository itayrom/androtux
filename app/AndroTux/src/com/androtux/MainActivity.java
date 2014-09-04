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

import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.content.DialogInterface;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

public class MainActivity extends FragmentActivity implements SensorEventListener {
	private static final int REQUEST_ENABLE_BT = 10;
	private static boolean _axis;
	
	private SensorManager _sManager;
	private BluetoothSocket _btSocket;
	private BluetoothAdapter _btAdapter;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        CommunicationHandler cHandler = CommunicationHandler.getInstance();
        
        // initialize bluetooth
		
        _btAdapter = BluetoothAdapter.getDefaultAdapter();
		if (_btAdapter == null) {
			cHandler.setIsBluetooth(false);
		} else {
			if (!_btAdapter.isEnabled()) {
			    Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
			    startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
			}
			
			cHandler.setIsBluetooth(true);
		    cHandler.setBluetoothAdapter(_btAdapter);
		}
        
        
        if (!cHandler.connect("192.168.192.54", 3490)) {
        	AlertDialog.Builder dlgAlert  = new AlertDialog.Builder(this);

        	dlgAlert.setMessage("Cound not connect to server");
        	dlgAlert.setTitle("Androtux");
        	dlgAlert.setCancelable(true);
        	dlgAlert.setPositiveButton("Ok",
        		    new DialogInterface.OnClickListener() {
        		        public void onClick(DialogInterface dialog, int which) {
        		        	System.exit(0); 
        		        }
        		    });
        	dlgAlert.create().show();
        }
        
        _sManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        
        _axis = false;
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
    @Override
    protected void onResume() {  
        super.onResume();  
        _sManager.registerListener(this, _sManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),SensorManager.SENSOR_DELAY_FASTEST);  
    }  
  
    @Override  
    protected void onStop() {  
        _sManager.unregisterListener(this);  
        super.onStop();  
    }
    
    @Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// TODO Auto-generated method stub
		
	}
    
    @Override  
    public void onSensorChanged(SensorEvent event) { 
    	if (_axis) {
    		String str = "a" + event.values[1]*200 + "," + event.values[0]*200;
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
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

	@Override
	protected void onDestroy() {
		CommunicationHandler.getInstance().disconnect();
		super.onDestroy();
	}
	
	public static void setAxis(boolean stat) {
		_axis = stat;
	}
	
	public void playAA(View view) {
		CommunicationHandler.getInstance().sendData("c");
	}
}
