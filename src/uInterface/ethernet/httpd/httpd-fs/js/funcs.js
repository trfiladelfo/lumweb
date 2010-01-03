function increase(id){
	var element = document.getElementById(id);
	var n_value = parseInt(element.value) + 1;
	if(n_value > 24)
		element.value = 0;
	else
		element.value = n_value;
}
			
function decrease(id){
	var element = document.getElementById(id);
	var n_value = parseInt(element.value) - 1;
	if(n_value < 0)
		element.value = 24;
	else
		element.value = n_value;
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
