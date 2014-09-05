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
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;

public class CommunicationHandler {
	private static CommunicationHandler _handler = null;
	
	private InetAddress _serverAddr;
	private int _port;
	private Socket _socket;
	private BluetoothSocket _btSocket;
	private BluetoothAdapter _btAdapter;
	
	private boolean _isBluetooth, _isWireless;
	
	private CommunicationHandler() {
		_isBluetooth = _isWireless = true;
	}
	
	public static CommunicationHandler getInstance() {
		if (_handler == null) {
			_handler = new CommunicationHandler();
		}
		
		return _handler;
	}
	
	public boolean connect(String ip, int port) {
		try {
			_serverAddr = InetAddress.getByName(ip);
			_port = port;
			new Thread(new ClientThread()).start();
		} catch (UnknownHostException e) {
			e.printStackTrace();
			return false;
		}
		
		return true;
	}
	
	public boolean connect(String macAddr) {
		return true;
	}
	
	public boolean disconnect() {
		return true;
	}
	
	public boolean sendData(String str) {
		try {
			PrintWriter out = new PrintWriter(new BufferedWriter(
	                new OutputStreamWriter(_socket.getOutputStream())),
	                true);
	        out.println(str);
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
		
		return true;
	}
	
	// Bluetooth setters/getters
	
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
	
	// <<-- end of Bluetooth setters/getters
	
	class ClientThread implements Runnable {
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
}

