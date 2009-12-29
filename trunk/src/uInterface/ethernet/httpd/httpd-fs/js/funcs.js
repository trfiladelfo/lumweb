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

