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

import android.app.Fragment;
import android.app.FragmentTransaction;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class TabBarFragment extends Fragment {
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
	        Bundle savedInstanceState) {
	    View rootView = inflater.inflate(R.layout.fragment_tabbar, container, false);
	    
	    ToggleButton tbKeyboard = (ToggleButton)rootView.findViewById(R.id.tbKeyboard);
	    ToggleButton tbMouse = (ToggleButton)rootView.findViewById(R.id.tbMouse);
	    ToggleButton tbGamepad = (ToggleButton)rootView.findViewById(R.id.tbGamepad);
	    ToggleButton tbSettings = (ToggleButton)rootView.findViewById(R.id.tbSettings);
	    ToggleButton tbAbout = (ToggleButton)rootView.findViewById(R.id.tbAbout);

	    
	    tbKeyboard.addOps(new Runnable() {
			@Override
			public void run() {
				FragmentTransaction transaction = getFragmentManager().beginTransaction();
				transaction.replace(R.id.item_fragment, new KeyboardFragment());
				transaction.addToBackStack(null);
				transaction.commit();
			}
		});
	    
	    tbMouse.addOps(new Runnable() {
			@Override
			public void run() {
				FragmentTransaction transaction = getFragmentManager().beginTransaction();
				transaction.replace(R.id.item_fragment, new MouseFragment());
				transaction.addToBackStack(null);
				transaction.commit();
			}
		});
	    
	    tbGamepad.addOps(new Runnable() {
			@Override
			public void run() {
				FragmentTransaction transaction = getFragmentManager().beginTransaction();
				transaction.replace(R.id.item_fragment, new GamepadFragment());
				transaction.addToBackStack(null);
				transaction.commit();
			}
		});
	    
	    tbSettings.addOps(new Runnable() {
			@Override
			public void run() {
				FragmentTransaction transaction = getFragmentManager().beginTransaction();
				transaction.replace(R.id.item_fragment, new SettingsFragment());
				transaction.addToBackStack(null);
				transaction.commit();
			}
		});
	    
	    tbAbout.addOps(new Runnable() {
			@Override
			public void run() {
				FragmentTransaction transaction = getFragmentManager().beginTransaction();
				transaction.replace(R.id.item_fragment, new AboutFragment());
				transaction.addToBackStack(null);
				transaction.commit();
			}
		});
	    
	    tbKeyboard.setImage(R.drawable.ic_action_keyboard);
	    tbMouse.setImage(R.drawable.ic_action_mouse);
	    tbGamepad.setImage(R.drawable.ic_action_gamepad);
	    tbSettings.setImage(R.drawable.ic_action_settings);
	    tbAbout.setImage(R.drawable.ic_action_about);
	    
	    tbKeyboard.addListener(tbMouse);
	    tbKeyboard.addListener(tbGamepad);
	    tbKeyboard.addListener(tbSettings);
	    tbKeyboard.addListener(tbAbout);
	    
	    tbMouse.addListener(tbKeyboard);
	    tbMouse.addListener(tbGamepad);
	    tbMouse.addListener(tbSettings);
	    tbMouse.addListener(tbAbout);
	    
	    tbGamepad.addListener(tbKeyboard);
	    tbGamepad.addListener(tbMouse);
	    tbGamepad.addListener(tbSettings);
	    tbGamepad.addListener(tbAbout);
	    
	    tbSettings.addListener(tbKeyboard);
	    tbSettings.addListener(tbMouse);
	    tbSettings.addListener(tbGamepad);
	    tbSettings.addListener(tbAbout);
	    
	    tbAbout.addListener(tbKeyboard);
	    tbAbout.addListener(tbMouse);
	    tbAbout.addListener(tbGamepad);
	    tbAbout.addListener(tbSettings);

	    tbKeyboard.setOn();
	    
	    return rootView;
	}
}