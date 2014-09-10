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
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public abstract class AbstractKey extends View {
	public AbstractKey(Context context) {
		super(context);
		init();
	}
	
	public AbstractKey(Context context, AttributeSet attrs) {
		super(context, attrs);
		init();
	}
	
	public AbstractKey(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init();
	}
	
	private void init() {
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
    	int action = event.getActionMasked();
    	
    	switch(action) {
    	case MotionEvent.ACTION_DOWN:
			onPress();
			setPressedParams();
			break;
    	case MotionEvent.ACTION_UP:
			onRelease();
			setReleasedParams();
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
	
	public abstract void drawKey(Canvas canvas);
	public abstract void setReleasedParams();
	public abstract void setPressedParams();
	public abstract void onRelease();
	public abstract void onPress();
}
