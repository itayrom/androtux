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
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;

public class TouchJoystick extends View {
	private Paint _paint;
	private Bitmap _bitmap;
	private Rect _rect;
	private float _x, _y;
	
	public TouchJoystick(Context context) {
		super(context);
		init();
	}
	
	public TouchJoystick(Context context, AttributeSet attrs) {
		super(context, attrs);
		init();
	}
	
	public TouchJoystick(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init();
	}
	
	private void init() {
		_paint = new Paint();
		_bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.joystick);
	}
	
	@Override
    public boolean onTouchEvent(MotionEvent event) {
		String str;
    	float x, y, offsetX, offsetY;
    	int action = event.getActionMasked();
    	//_x = getLeft() + getWidth()/2;
    	//_y = getTop() + getHeight()/2;
    	switch(action) {
    	case MotionEvent.ACTION_DOWN:
    		_x = event.getAxisValue(MotionEvent.AXIS_X);
    		_y = event.getAxisValue(MotionEvent.AXIS_Y);
    		break;
    	case MotionEvent.ACTION_MOVE:
    		x = event.getAxisValue(MotionEvent.AXIS_X);
    		y = event.getAxisValue(MotionEvent.AXIS_Y);
    		
    		offsetX = (x - _x) * 10;
    		offsetY = (y - _y) * 10;
    		
    		str = "a" + offsetX + "," + offsetY;
            CommunicationHandler.getInstance().sendData(str);
    		break;
    	case MotionEvent.ACTION_CANCEL:
    	case MotionEvent.ACTION_UP:
    		str = "a0,0";
            CommunicationHandler.getInstance().sendData(str);
    		break;
    	}
    	
    	return true;
    }
	
	@Override
	protected void onDraw(Canvas canvas) {
		_rect = new Rect(0, 0, getWidth(), getHeight());
		canvas.drawBitmap(_bitmap, null, _rect, null);
	}
}
