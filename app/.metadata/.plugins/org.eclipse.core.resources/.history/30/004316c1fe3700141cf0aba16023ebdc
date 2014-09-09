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
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Align;
import android.graphics.Paint.Style;
import android.graphics.Rect;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;

public class GamepadTouchKey extends AbstractKey {
	private String _keyType;
	private String _keyCode;
	private Paint _paint;
	private String _text;
	private int _keyColor;
	private int _growFactor;
	private int _color;
	
	public GamepadTouchKey(Context context) {
		super(context);
		init();
	}
	
	public GamepadTouchKey(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(attrs);
	}
	
	public GamepadTouchKey(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(attrs);
	}
	
	private void init() {
		_paint = new Paint();
		setReleasedParams();
	}
	
	private void init(AttributeSet attrs) {
		TypedArray a=getContext().obtainStyledAttributes(attrs, R.styleable.TouchKey);
		_text = a.getString(R.styleable.TouchKey_android_text);
		setKeyType(a.getString(R.styleable.TouchKey_keytype));
		setKeyCode(a.getString(R.styleable.TouchKey_keycode));
		_color = a.getColor(R.styleable.TouchKey_color, Color.GRAY);
		a.recycle();
		init();
	}
	
	@Override
	public void drawKey(Canvas canvas) {
		_paint.setStyle(Style.FILL_AND_STROKE);
		
		_paint.setColor(Color.BLUE);
		//canvas.drawRect(0, 0, getWidth(), getHeight(), _paint);
		
		_paint.setColor(Color.BLACK);
		canvas.drawCircle(getWidth()/2, getHeight()/2, getWidth()/3, _paint);
		
		_paint.setColor(_keyColor);
		canvas.drawCircle(getWidth()/2, getHeight()/2, getWidth()/3 - 5, _paint);
		
		_paint.setColor(Color.BLACK);
		_paint.setTextSize(getWidth()/3 + _growFactor);
		canvas.drawText(_text, this.getWidth()/3+10, this.getHeight()/2+15, _paint);
	}

	@Override
	public void setReleasedParams() {
		_keyColor = _color;
		_growFactor = 0;
	}

	@Override
	public void setPressedParams() {
		_keyColor = Color.YELLOW;
		_growFactor = 3;
	}

	@Override
	public void onRelease() {
		CommunicationHandler.getInstance().sendData("k" + getKeyType() + "0" + getKeyCode());
	}

	@Override
	public void onPress() {
		CommunicationHandler.getInstance().sendData("k" + getKeyType() + "1" + getKeyCode());
	}
	
	// setters/getters
	public void setKeyType(String keyType) {
		_keyType = keyType;
	}
	
	public String getKeyType() {
		return _keyType;
	}
	
	public void setKeyCode(String keyCode) {
		_keyCode = keyCode;
	}
	
	public String getKeyCode() {
		return _keyCode;
	}
	// <-- end of setters/getters
}
