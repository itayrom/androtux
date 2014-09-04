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

import java.util.Set;
import android.app.Fragment;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;

public class SettingsFragment extends Fragment {
	private static final int SERVER_PORT = 3490;
	//private ListView _lvDiscovered;
	private ListView _lvPaired;
	private EditText _txtWirelessIp;
	//private ClientArrayAdapter _arrayAdapter;
	//private ArrayAdapter<BluetoothClient> _arrayAdapter;
	private BluetoothAdapter _bluetoothAdapter;
	CommunicationHandler _cHandler;
	
	/*private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
		@Override
	    public void onReceive(Context context, Intent intent) {
			ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(getActivity().getApplicationContext(), R.layout.fragment_settings);
	        String action = intent.getAction();
	        // When discovery finds a device
	        if (BluetoothDevice.ACTION_FOUND.equals(action)) {
	            // Get the BluetoothDevice object from the Intent
	            BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
	            // Add the name and address to an array adapter to show in a ListView
	            arrayAdapter.add(device.getName() + "\n" + device.getAddress());
	        }
	    }
	};*/
	
	/**
	 * A placeholder fragment containing a simple view.
	 */
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
	        Bundle savedInstanceState) {
	    View rootView = inflater.inflate(R.layout.fragment_settings, container, false);
	    
	    _cHandler = CommunicationHandler.getInstance();
	    _bluetoothAdapter = CommunicationHandler.getInstance().getBluetoohAdapter();
	    //_lvDiscovered = (ListView)rootView.findViewById(R.id.lvDiscovered);
	    _lvPaired = (ListView)rootView.findViewById(R.id.lvPaired);
	    _txtWirelessIp = (EditText)rootView.findViewById(R.id.txtWirelessIp);
	    //_arrayAdapter = new ArrayAdapter<BluetoothClient>(rootView.getContext(), android.R.layout.simple_list_item_1);
	    
	   /* ListView lv = (ListView)rootView.findViewById(R.id.lvDiscovered);
	    ClientArrayAdapter adapter = new ClientArrayAdapter(getActivity(), R.layout.bluetooth_list_item);
	    BluetoothClient c1 = new BluetoothClient("Poop", "aa:bb:cc:dd:ee:ff");
	    adapter.add(c1);
	    lv.setAdapter(adapter);*/
	    
	   /* ListView lv = (ListView)rootView.findViewById(R.id.lvDiscovered);
	    TextView tv = new TextView(getActivity());
        tv.setText("Poop\naa:bb:cc:dd:ee:ff");
        lv.add*/
	    

	    /*_lvDiscovered.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				BluetoothClient item = (BluetoothClient) _lvDiscovered.getItemAtPosition(position);
				
				_cHandler.setConnectionType(ConnectionType.BLUETOOTH);
				_cHandler.setMac(item.getAddress());
				_cHandler.connect();
			}
		});*/
	    
	    /*TouchButton btnDiscover = (TouchButton)rootView.findViewById(R.id.btnDiscover);
		btnDiscover.addOps(new Runnable() {
			@Override
			public void run() {
                final BroadcastReceiver mReceiver = new BroadcastReceiver() 
                { 
                    public void onReceive(Context context, Intent intent) 
                    {
                        String action = intent.getAction(); 
                        if (BluetoothDevice.ACTION_FOUND.equals(action)) 
                        {
	                        BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
	                        BluetoothClient client = new BluetoothClient(device.getName(), device.getAddress());
	                        _arrayAdapter.add(client);
	                        _lvDiscovered.removeAllViewsInLayout();
	                        _lvDiscovered.setAdapter(_arrayAdapter);
                        }
                    }    
                };

                //String aDiscoverable = BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE;
                //startActivityForResult(new Intent(aDiscoverable),DISCOVERY_REQUEST);
                IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND); 
                getActivity().getApplicationContext().registerReceiver(mReceiver, filter); 
                _bluetoothAdapter.startDiscovery();
			}
		});
	    */
	    
	    TouchButton btnConnectWireless = (TouchButton)rootView.findViewById(R.id.btnConnectWireless);
	    btnConnectWireless.addOps(new Runnable() {
			@Override
			public void run() {
                final BroadcastReceiver mReceiver = new BroadcastReceiver() 
                { 
                    public void onReceive(Context context, Intent intent) 
                    {
                        String action = intent.getAction(); 
                        if (BluetoothDevice.ACTION_FOUND.equals(action)) 
                        {
                        	String ip = _txtWirelessIp.getText().toString();
                        	_cHandler.setConnectionType(ConnectionType.WIRELESS);
                        	_cHandler.setIp(ip);
                        	_cHandler.setPort(SERVER_PORT);
                        	if (!_cHandler.connect()) {
                        		Toast.makeText(getActivity(), "Could not connect to " + ip + " via Wireless", Toast.LENGTH_SHORT).show();
                        	}
                        }
                    }    
                };

                //String aDiscoverable = BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE;
                //startActivityForResult(new Intent(aDiscoverable),DISCOVERY_REQUEST);
                IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND); 
                getActivity().getApplicationContext().registerReceiver(mReceiver, filter); 
                _bluetoothAdapter.startDiscovery();
			}
		});
	    
		ArrayAdapter<BluetoothClient> arrayAdapter = new ArrayAdapter<BluetoothClient>(rootView.getContext(), android.R.layout.simple_list_item_1);
		Set<BluetoothDevice> pairedDevices = _cHandler.getBluetoohAdapter().getBondedDevices();
		// If there are paired devices
		if (pairedDevices.size() > 0) {
		    // Loop through paired devices
		    for (BluetoothDevice device : pairedDevices) {
		    	BluetoothClient client = new BluetoothClient(device.getName(), device.getAddress());
		    	arrayAdapter.add(client);
                _lvPaired.removeAllViewsInLayout();
                _lvPaired.setAdapter(arrayAdapter);
		    }
	    }
		
		_lvPaired.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				BluetoothClient item = (BluetoothClient) _lvPaired.getItemAtPosition(position);
				
				_cHandler.setConnectionType(ConnectionType.BLUETOOTH);
				_cHandler.setMac(item.getAddress());
				if (!_cHandler.connect()) {
					Toast.makeText(getActivity(), "Could not connect to " + item.getAddress() + " via Bluetooth", Toast.LENGTH_SHORT).show();
				}
			}
		});
		     
	    /*final ArrayAdapter<String>_arrayAdapter = new ArrayAdapter<String>(rootView.getContext(), R.layout.bluetooth_list_item);
		final ListView _lvDiscovered = (ListView)rootView.findViewById(R.id.lvDiscovered);
		
	    TouchButton btnDiscover = (TouchButton)rootView.findViewById(R.id.btnDiscover);
	    
		btnDiscover.addOps(new Runnable() {
			private BluetoothAdapter _bluetoothAdapter;
			
			@Override
			public void run() {
				_bluetoothAdapter = CommunicationHandler.getInstance().getBluetoohAdapter();

                final BroadcastReceiver mReceiver = new BroadcastReceiver() 
                { 
                    public void onReceive(Context context, Intent intent) 
                    {
                        String action = intent.getAction(); 
                        // When discovery finds a device 
                        if (BluetoothDevice.ACTION_FOUND.equals(action)) 
                        {
	                        // Get the BluetoothDevice object from the Intent 
	                        BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
	                        BluetoothClient client = new BluetoothClient(device.getName(), device.getAddress());
	                        //_arrayAdapter.add(device.getName() + "\n" + device.getAddress());
	                        //Toast.makeText(getActivity().getApplicationContext(), device.getName() + "\n" + device.getAddress(), 3).show();
	                        //TextView tv = new TextView(getActivity());
	                        //tv.setText(device.getName() + "\n" + device.getAddress());
	                        //_lvDiscovered.addView((View)tv);
	                        _lvDiscovered.setAdapter(_arrayAdapter);
                        }
                    }    
                };

                //String aDiscoverable = BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE;
                //startActivityForResult(new Intent(aDiscoverable),DISCOVERY_REQUEST);
                IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND); 
                getActivity().getApplicationContext().registerReceiver(mReceiver, filter); 
                _bluetoothAdapter.startDiscovery();
			}
		});
		// Register the BroadcastReceiver
		//IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
		//	    	context.registerReceiver(mReceiver, filter); // Don't forget to unregister during onDestroy
		*/
		return rootView;
	}
	
}
