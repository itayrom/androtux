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
import android.view.View;

public class WheelToggleButton extends View{
	private boolean _pressed;
	private Paint _paint;
	private int _keyColor;
	private Bitmap _wheel;
	private Rect _rect;
	
	public WheelToggleButton(Context context) {
		super(context);
		init();
	}
	
	public WheelToggleButton(Context context, AttributeSet attrs) {
		super(context, attrs);
		init();
	}
	
	public WheelToggleButton(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init();
	}
	
	private void init() {
		_paint = new Paint();
		_pressed = false;
		_wheel = BitmapFactory.decodeResource(getResources(), R.drawable.wheel);
		setOffParams();
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
    	int action = event.getActionMasked();
    	
    	switch(action) {
    	case MotionEvent.ACTION_DOWN:
    		_pressed = ! _pressed;
    		if (_pressed)
    			setOnParams();
    		else {
    			CommunicationHandler.getInstance().sendData("a0,0");
    			setOffParams();
    		}
    		
			MainActivity.setAxis(_pressed);
			break;
    	case MotionEvent.ACTION_UP:
			break;
    	}
    	
    	this.invalidate();
		return true;
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		drawKey(canvas);
	}
	
	public void drawKey(Canvas canvas) {
		_rect = new Rect(5, 5, getWidth()-5, getHeight()-5);
		_paint.setColor(_keyColor);
		canvas.drawCircle(getWidth()/2, getHeight()/2, getWidth()/2, _paint);
		canvas.drawBitmap(_wheel, null, _rect, null);
		/*_paint.setStyle(Style.FILL_AND_STROKE);
		
		_paint.setColor(Color.BLUE);
		//canvas.drawRect(0, 0, getWidth(), getHeight(), _paint);
		
		_paint.setColor(Color.BLACK);
		canvas.drawCircle(getWidth()/2, getHeight()/2, getWidth()/3, _paint);
		
		_paint.setColor(_keyColor);
		canvas.drawCircle(getWidth()/2, getHeight()/2, getWidth()/3 - 5, _paint);
		
		_paint.setColor(Color.WHITE);
		_paint.setTextSize(getWidth()/3);
		canvas.drawText("W", this.getWidth()/3+10, this.getHeight()/2+15, _paint);*/
	}
	
	public void setOnParams() {
		_keyColor = Color.GREEN;
	}
	
	public void setOffParams() {
		_keyColor = Color.GRAY;
	}
	
}
