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

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public class ToggleButton extends View implements IToggleable {
	private Paint _paint;
	private boolean _pressed;
	private Bitmap _icon;
	private RectF _rect;
	private int _bgColor, _onColor, _offColor;
	private boolean _isTab;
	
	private List<Runnable> _onOps;
	private List<Runnable> _offOps;
	private List<IToggleable> _listeners;
	
	public ToggleButton(Context context) {
		super(context);
		init();
	}
	
	public ToggleButton(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(attrs);
	}
	
	public ToggleButton(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(attrs);
	}
	
	protected void init() {
		_onOps = new ArrayList<Runnable>();
		_offOps = new ArrayList<Runnable>();
		_listeners = new ArrayList<IToggleable>();
		_paint = new Paint();
		_pressed = false;
		_icon = BitmapFactory.decodeResource(getResources(), R.drawable.ic_action_keyboard);
		setOffParams();
	}
	
	private void init(AttributeSet attrs) {
		TypedArray a=getContext().obtainStyledAttributes(attrs, R.styleable.ToggleButton);
		_offColor = a.getColor(R.styleable.ToggleButton_offColor, Color.RED);
		_onColor = a.getColor(R.styleable.ToggleButton_onColor, Color.GREEN);
		_isTab = a.getBoolean(R.styleable.ToggleButton_isTab, false);
		
		a.recycle();
		init();
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
    	int action = event.getActionMasked();
    	
    	switch(action) {
    	case MotionEvent.ACTION_DOWN:
    		if (!_isTab || (_isTab && !_pressed))
    			_pressed = ! _pressed;
    		
    		if (_pressed) {
    			setOn();
    		} else {
    			setOff();
    		}
			break;
    	case MotionEvent.ACTION_UP:
			break;
    	}
    	
    	this.invalidate();
		return true;
	}
	
	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		/*int width = MeasureSpec.getSize(widthMeasureSpec);
		int height = width * _icon.getHeight() / _icon.getWidth();*/
		int height = MeasureSpec.getSize(heightMeasureSpec);
		int width = height * _icon.getWidth() / _icon.getHeight();
		setMeasuredDimension(width, height);
	}
	
	private void runOn() {
		for (Runnable current : _onOps) {
			current.run();
		}
	}
	
	private void runOff() {
		for (Runnable current : _offOps) {
			current.run();
		}
	}
	
	public void addOnOps(Runnable op) {
		_onOps.add(op);
	}
	
	public void addOffOps(Runnable op) {
		_offOps.add(op);
	}
	
	public void addListener(IToggleable listener) {
		_listeners.add(listener);
	}
	
	public void setImage(int id) {
		_icon = BitmapFactory.decodeResource(getResources(), id);
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		_rect = new RectF(5, 5, getWidth()- 5, getHeight() - 5);
		_paint.setColor(_bgColor);
		canvas.drawRoundRect(_rect, 15, 15, _paint);
		canvas.drawBitmap(_icon, null, _rect, null);
	}
	
	protected void setOffParams() {
		_bgColor = _offColor;
	}
	
	protected void setOnParams() {
		_bgColor = _onColor;
	}

	@Override
	public void setOn() {
		_pressed = true;
		setOnParams();
		invalidate();
		
		for (IToggleable current : _listeners) {
			current.setOff();
		}
		
		runOn();
	}

	@Override
	public void setOff() {
		_pressed = false;
		setOffParams();
		invalidate();
		
		runOff();
	}
}
