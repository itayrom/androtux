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

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;

public class CommunicationHandler {
	private static final int REQUEST_ENABLE_BT = 1;
	//private final UUID APPUUID = UUID.fromString("7335b5f2-ec43-4e53-8419-07657a9a8d52");
	private final UUID APPUUID = UUID.fromString("07293db4-a323-4e07-8b8b-250b340e42a4");
	
	private static CommunicationHandler _handler = null;
	
	private InetAddress _serverAddr;
	private String _ip;
	private String _mac;
	private int _port;
	private Socket _socket;
	private BluetoothSocket _btSocket;
	private BluetoothAdapter _btAdapter;
	private BluetoothDevice _btDevice;
	private ConnectionType _connectionType;
	private Activity _activity;
	
	private boolean _isBluetooth;
	
	private CommunicationHandler() {
		_isBluetooth = true;
		_connectionType = ConnectionType.NONE;
	}
	
	public static CommunicationHandler getInstance() {
		if (_handler == null) {
			_handler = new CommunicationHandler();
		}
		
		return _handler;
	}
	
	public void initialize() {
		_btAdapter = BluetoothAdapter.getDefaultAdapter();
		if (_btAdapter == null) {
			setIsBluetooth(false);
		} else {
			setIsBluetooth(true);
		    setBluetoothAdapter(_btAdapter);
		}
	}
	
	public boolean connect() {
		switch (_connectionType) {
		case WIRELESS:
			try {
				if (_socket != null &&_socket.isConnected())
					_socket.close();
				if (_btSocket != null &&_btSocket.isConnected())
					_btSocket.close();
				_serverAddr = InetAddress.getByName(_ip);
				new Thread(new WirelessThread()).start();
			} catch (UnknownHostException e) {
				e.printStackTrace();
				return false;
			} catch (IOException e) {
				e.printStackTrace();
			}
			break;
		case BLUETOOTH:
			try {
				if (_socket != null &&_socket.isConnected())
					_socket.close();
				if (_btSocket != null &&_btSocket.isConnected())
					_btSocket.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			
			if (!_btAdapter.isEnabled()) {
			    Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
			    _activity.startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
			}
			new Thread(new BluetoothThread()).start();
			break;
		case NONE:
			return false;
		}
		
		return true;
	}
	
	public boolean disconnect() {
		switch (_connectionType) {
		case WIRELESS:
			try {
				if (_socket != null)
					_socket.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			break;
		case BLUETOOTH:
			try {
				if (_btSocket != null)
					_btSocket.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			break;
		case NONE:
			break;
		}
		return true;
	}
	
	public boolean sendData(String str) {
		switch (_connectionType) {
		case WIRELESS:
			try {
				PrintWriter out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(_socket.getOutputStream())), true);
		        out.println(str);
	        } catch (IOException e) {
	            e.printStackTrace();
	            return false;
	        } catch (Exception e) {
	            e.printStackTrace();
	            return false;
	        }
			break;
		case BLUETOOTH:
			try {
				/*PrintWriter out = new PrintWriter(new OutputStreamWriter(_btSocket.getOutputStream()));
				out.print(str);*/
				OutputStream out = _btSocket.getOutputStream();
				byte[] msg = (str + " ").getBytes();
				msg[msg.length - 1] = 0;
				out.write(msg);
			} catch (IOException e) {
	            e.printStackTrace();
	            return false;
	        } catch (Exception e) {
	            e.printStackTrace();
	            return false;
	        }
			break;
		case NONE:
			return false;
		}
		
		return true;
	}
	
	// setters/getters
	
	public void setConnectionType(ConnectionType type) {
		_connectionType = type;
	}
	
	public ConnectionType getConnectionType() {
		return _connectionType;
	}
	
	public void setIp(String ip) {
		_ip = ip;
	}
	
	public String getIp() {
		return _ip;
	}
	
	public void setPort(int port) {
		_port = port;
	}
	
	public int getPort() {
		return _port;
	}
	
	public void setMac(String mac) {
		_mac = mac;
	}
	
	public String getMac() {
		return _mac;
	}
	
	public void setBluetoothAdapter(BluetoothAdapter adapter) {
		_btAdapter = adapter;
	}
	
	public BluetoothAdapter getBluetoohAdapter() {
		return _btAdapter;
	}
	
	public void setIsBluetooth(boolean isBt) {
		_isBluetooth = isBt;
	}
	
	public boolean getIsBluetooth() {
		return _isBluetooth;
	}
	
	public void setActivity(Activity activity) {
		_activity = activity;
	}
	
	// <<-- end of setters/getters
	
	class WirelessThread implements Runnable {
	    @Override
	    public void run() {

	        try {
	            _socket = new Socket(_serverAddr, _port);

	        } catch (UnknownHostException e1) {
	            e1.printStackTrace();
	        } catch (IOException e1) {
	            e1.printStackTrace();
	        }

	    }
	}
	
	class BluetoothThread implements Runnable {

		@Override
		public void run() {
			try {
	        	_btDevice = _btAdapter.getRemoteDevice(_mac);
	        	_btSocket = _btDevice.createInsecureRfcommSocketToServiceRecord(APPUUID);
	        	_btAdapter.cancelDiscovery();
	            _btSocket.connect();
	        } catch (IOException e0) {
	            // Unable to connect; close the socket and get out
	            try {
	            	_btSocket.close();
	            } catch (IOException e1) {
	            	e1.printStackTrace();
	            }
	        }
		}
	}
}

