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

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.Button;

public class Touchpad extends Button {
	boolean movementDetected, isScroll;
	private float _x, _y;
	
	public Touchpad(Context context) {
		super(context);
	}
	
	public Touchpad(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	
	public Touchpad(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}
	
	@Override
    public boolean onTouchEvent(MotionEvent event) {
		String str = "";
    	float x, y, offsetX, offsetY;
    	int action = event.getActionMasked();
    	
    	switch(action) {
    	case MotionEvent.ACTION_POINTER_DOWN:
    	case MotionEvent.ACTION_DOWN:
    		movementDetected = false;
    		_x = event.getAxisValue(MotionEvent.AXIS_X);
    		_y = event.getAxisValue(MotionEvent.AXIS_Y);
    		isScroll = (event.getPointerCount() == 2) ? true : false;
    		break;
    	case MotionEvent.ACTION_POINTER_UP:
    	case MotionEvent.ACTION_UP:
    		isScroll = false;
    		
    		if (!movementDetected) {
	    		x = event.getAxisValue(MotionEvent.AXIS_X);
	    		y = event.getAxisValue(MotionEvent.AXIS_Y);
	    		
	    		if (_x == x && _y == y) {
	    			str = "km1272";
	    			CommunicationHandler.getInstance().sendData(str);
	    			str = "km0272";
	    			CommunicationHandler.getInstance().sendData(str);
	    		}
    		}
    		break;
    	case MotionEvent.ACTION_MOVE:
    		movementDetected = true;
    		x = event.getAxisValue(MotionEvent.AXIS_X);
    		y = event.getAxisValue(MotionEvent.AXIS_Y);
    		
    		offsetX = x - _x;
    		
    		if (isScroll) {
    			offsetY = (_y - y) / 5;
	    		
	    		if (Math.abs(offsetY) > 0.1) {
		    		str = "s" + offsetY;
	    		}
    		}else{
    			offsetY = y - _y;
    			
    			//if (Math.abs(offsetX) > 0.1 && Math.abs(offsetY) > 0.1) {
    	    		str = "r" + offsetX + "," + offsetY;
        		//}
    		}
    		
    		CommunicationHandler.getInstance().sendData(str);
    		
    		_x = x;
    		_y = y;
    		
    		
    		
    		
    		/*if (isScroll) {
	    		movementDetected = true;
	    		y = event.getAxisValue(MotionEvent.AXIS_Y);
	    		
	    		offsetY = (_y - y) / 5;
	    		
	    		if (Math.abs(offsetY) > 0.1) {
		    		str = "s" + offsetY;
		            CommunicationHandler.getInstance().sendData(str);
		            
		    		_y = y;
	    		}
    		}*/
    		break;
/*    	case MotionEvent.ACTION_DOWN:
    		movementDetected = false;
    		_x = event.getAxisValue(MotionEvent.AXIS_X);
    		_y = event.getAxisValue(MotionEvent.AXIS_Y);
    		break;
    	case MotionEvent.ACTION_MOVE:
    		movementDetected = true;
    		x = event.getAxisValue(MotionEvent.AXIS_X);
    		y = event.getAxisValue(MotionEvent.AXIS_Y);
    		
    		offsetX = x - _x;
    		offsetY = y - _y;
    		
    		if (Math.abs(offsetX) > 0.1 && Math.abs(offsetY) > 0.1) {
	    		str = "r" + offsetX + "," + offsetY;
	            CommunicationHandler.getInstance().sendData(str);
	            
	    		_x = x;
	    		_y = y;
    		}
    		break;
    	case MotionEvent.ACTION_UP:
    		if (!movementDetected) {
	    		x = event.getAxisValue(MotionEvent.AXIS_X);
	    		y = event.getAxisValue(MotionEvent.AXIS_Y);
	    		
	    		if (_x == x && _y == y) {
	    			str = "km1272";
	    			CommunicationHandler.getInstance().sendData(str);
	    			str = "km0272";
	    			CommunicationHandler.getInstance().sendData(str);
	    		}
    		}
    		break;*/
    	case MotionEvent.ACTION_CANCEL:
    		break;
    	}
    	
    	return true;
    }
}
