package com.androtux;

public class SyncBoolean {
	private boolean _val;
	
	public SyncBoolean(boolean val) {
		_val = val;
	}
	
	public void setVal(boolean val) {
		_val = val;
	}
	
	public boolean getVal() {
		return _val;
	}
}
