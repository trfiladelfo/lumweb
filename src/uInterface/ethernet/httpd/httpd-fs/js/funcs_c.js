/*compressed jx.js + func.js */
jx={getHTTPObject:function(){var A=false;if(typeof ActiveXObject!="undefined"){try{A=new ActiveXObject("Msxml2.XMLHTTP")}catch(C){try{A=new ActiveXObject("Microsoft.XMLHTTP")}catch(B){A=false}}}else{if(window.XMLHttpRequest){try{A=new XMLHttpRequest()}catch(C){A=false}}}return A},load:function(url,callback,format){var http=this.init();if(!http||!url){return }if(http.overrideMimeType){http.overrideMimeType("text/xml")}if(!format){var format="text"}format=format.toLowerCase();var now="uid="+new Date().getTime();url+=(url.indexOf("?")+1)?"&":"?";url+=now;http.open("GET",url,true);http.onreadystatechange=function(){if(http.readyState==4){if(http.status==200){var result="";if(http.responseText){result=http.responseText}if(format.charAt(0)=="j"){result=result.replace(/[\n\r]/g,"");result=eval("("+result+")")}if(callback){callback(result)}}else{if(error){error(http.status)}}}};http.send(null)},init:function(){return this.getHTTPObject()}}

function inc(id, max, min){
	var element = document.getElementById(id);
	var n_value = parseInt(element.value) + 1;
	if(max != -1 && min != -1){
		if(n_value > max)
			element.value = min;
	}else
		element.value = n_value;
}
			
function dec(id, max, min){
	var element = document.getElementById(id);
	var n_value = parseInt(element.value) - 1;
	if(max != -1 && min != -1){
		if(n_value < min)
			element.value = max;
	}else
		element.value = n_value;
}

function addB(id, max, min){
	document.write("<br /><input type=\"button\" value=\"+\" onclick=\"inc('"+id+"',"+max+","+min+");\" /><input type=\"button\" value=\"-\" onclick=\"dec('"+id+"',"+max+","+min+");\" />");
	
}

function submit_form(id){
	var get_string = get_form_values();
	var form = document.getElementById('form-'+id);

	jx.load(form.action+get_string+"ajax=1",function(data){
		var return_container = document.getElementById('container-'+id);
		return_container.innerHTML = data;
	});

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
