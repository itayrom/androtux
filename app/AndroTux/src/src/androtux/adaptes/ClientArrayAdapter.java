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

package src.androtux.adaptes;

import com.androtux.BluetoothClient;
import com.androtux.R;

import android.app.Activity;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

public class ClientArrayAdapter extends ArrayAdapter<BluetoothClient>{
	private Context _context;
	private int _resource;
	
	public ClientArrayAdapter(Context context, int resource) {
		super(context, resource);
		
		_resource = resource;
        _context = context;
	}
	
    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        if(convertView==null){
            LayoutInflater inflater = ((Activity)_context).getLayoutInflater();
            convertView = inflater.inflate(R.layout.simple_list_item, parent, false);
        }

        BluetoothClient item = getItem(position);

        TextView textViewItem = (TextView) convertView.findViewById(R.id.textViewItem);
        textViewItem.setText(item.getName() + "\n" + item.getAddress());
        textViewItem.setTag(item.getAddress());

        return convertView;

    }
    
    @Override
    public void add(BluetoothClient c) {
    	super.add(c);
    }
}
