jx={getHTTPObject:function(){var A=false;if(typeof ActiveXObject!="undefined"){try{A=new ActiveXObject("Msxml2.XMLHTTP")}catch(C){try{A=new ActiveXObject("Microsoft.XMLHTTP")}catch(B){A=false}}}else{if(window.XMLHttpRequest){try{A=new XMLHttpRequest()}catch(C){A=false}}}return A},load:function(url,callback,format){var http=this.init();if(!http||!url){return }if(http.overrideMimeType){http.overrideMimeType("text/xml")}if(!format){var format="text"}format=format.toLowerCase();var now="uid="+new Date().getTime();url+=(url.indexOf("?")+1)?"&":"?";url+=now;http.open("GET",url,true);http.onreadystatechange=function(){if(http.readyState==4){if(http.status==200){var result="";if(http.responseText){result=http.responseText}if(format.charAt(0)=="j"){result=result.replace(/[\n\r]/g,"");result=eval("("+result+")")}if(callback){callback(result)}}else{if(error){error(http.status)}}}};http.send(null)},init:function(){return this.getHTTPObject()}}

function valueOut (elementValue, value, decimal) {
	/*if (decimal != 0) {
		elementValue.value = parseFloat(value) / 10;
	} else { 
		elementValue.value = parseInt(value);
	}*/

	elementValue.value = value;
}

function inc(id, maxVal, minValm, increment, decimal){
	var element = document.getElementById(id);
	var n_value = parseInt(element.value) + increment;
	
	valueOut (element, n_value, decimal); 
	if(maxVal != -1 && minVal != -1){
		if(n_value > maxVal)
			valueOut (element, minVal, decimal);
	}
}

function dec(id, maxVal, minVal, increment, decimal){
	var element = document.getElementById(id);
	var n_value = parseInt(element.value) - increment;
	
	valueOut (element, n_value, decimal); 
	if(maxVal != -1 && minVal != -1){
		if(n_value < minVal)
			valueOut (element, maxVal, decimal);
	}		
}

function addB(id, maxParam, minParam, incrementParam, decimal){
	
	var maxVal = parseInt (maxParam);
	var minVal = parseInt (minParam);
	var increment =  parseInt (incrementParam);
	/*if (decimal == 0) {
		maxVal /= 10;
		minVal /= 10;
		increment /= 10;
	}*/
	document.write("<input type=\"button\" value=\"+\" onclick=\"inc('"+id+"',"+maxVal+","+minVal+", "+increment+", "+decimal+");\" />"+
		       "<input type=\"button\" value=\"-\" onclick=\"dec('"+id+"',"+maxVal+","+minVal+", "+increment+", "+decimal+");\" />");

}

function submit_form(form){
	var get_string = get_form_values();
	//var form = document.getElementById('form-'+id);

	jx.load(form.action+get_string+"ajax=1",function(data){
		var return_container = document.getElementById("container-" + form.name);
		return_container.innerHTML = data;
	});
	return false;
}

function getElementsByClass( searchClass, domNode, tagName) {
	if (domNode == null) domNode = document;
	if (tagName == null) tagName = '*';
	var el = new Array();
	var tags = domNode.getElementsByTagName(tagName);
	var tcl = " "+searchClass+" ";
	for(i=0,j=0; i<tags.length; i++) {
		var test = " " + tags[i].className + " ";
		if (test.indexOf(tcl) != -1)
			el[j++] = tags[i];
	}
	return el;
}
 

function get_form_values(){
	var get_string = "?";
	var fields = getElementsByClass('fi');

	for(i=0; i<fields.length; i++){
		get_string += fields[i].name + "=" + fields[i].value +"&";		
	}
	return get_string;
}