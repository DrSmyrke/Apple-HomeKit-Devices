var formRequest					= makeHttpObject();
var formViewBoxID				= "";
var brightMax					= 100;

//-------------------------------------------------------------------------------------------------------ы
function makeHttpObject()
{
	try {return new XMLHttpRequest();}

	catch (error) {}
	try {return new ActiveXObject("Msxml2.XMLHTTP");}
	catch (error) {}

	try {return new ActiveXObject("Microsoft.XMLHTTP");}
	catch (error) {}
	throw new Error("Could not create HTTP request object.");
}

//-------------------------------------------------------------------------------------------------------
function onUpdateParam( input )
{
	var statusBoxObj	= document.getElementById( input.name + "_status" );
	if( statusBoxObj == undefined ){
		console.log( "form.js: onUpdateParam: statusBoxObj not found", input.name );
		return;
	}

	var type			= input.lang;
	var name			= input.name;
	var action			= input.getAttribute( "data-action" );
	var cmd				= input.getAttribute( "data-cmd" );
	var id				= input.getAttribute( "data-id" );

	if( action == undefined || action == "" ){
		console.log( "form.js: onUpdateParam: action", action );
		return;
	}

	formRequest.open( "POST", action, false );
	formRequest.setRequestHeader( 'Content-type','application/x-www-form-urlencoded' );
	var string			= "";

	if( cmd != "" )		string += "cmd=" + cmd;
	if( id != "" )		string += "&data[id]=" + id;
	if( type != "" )	string += "&data[type]=" + type;
	if( name != "" )	string += "&data[param]=" + name;

	string += "&data[value]=" + input.value;

	formRequest.send( string );

	statusBoxObj.innerHTML = formRequest.responseText;
}

//-------------------------------------------------------------------------------------------------------
function changeParam( form, viewBoxID )
{
	var verbose = true;
	var viewBoxObj = document.getElementById( viewBoxID );
	if( viewBoxObj == undefined ){
		viewBoxObj = document.getElementById( formViewBoxID );
		if( viewBoxObj == undefined ){
			console.warn( "form.js: changeParam: viewBoxObj not found" );
			verbose = false;
		}
	}

	var arrayInput			= form.getElementsByTagName("INPUT");
	var arraySelect			= form.getElementsByTagName("SELECT");
	var arrayTextarea		= form.getElementsByTagName("TEXTAREA");
	var finalData			= Array();
	var finalString			= "";

	for( i = 0; i < arrayInput.length; i++ ){
		if( arrayInput[i].name == "" ) continue;
		if( arrayInput[i].type == "radio" && !arrayInput[i].checked ) continue;
		if( arrayInput[i].value != "" ) finalData.push( arrayInput[i].name + "=" + arrayInput[i].value );
	}
	for( i = 0; i < arraySelect.length; i++ ){
		if( arraySelect[i].name == "" ) continue;
		if( arraySelect[i].value != "" ) finalData.push( arraySelect[i].name + "=" + arraySelect[i].value );
	}
	for( i = 0; i < arrayTextarea.length; i++ ){
		if( arrayTextarea[i].name == "" ) continue;
		if( arrayTextarea[i].value != "" ) finalData.push( arrayTextarea[i].name + "=" + arrayTextarea[i].value );
	}

	finalString = finalData.join( "&" );

	formRequest.open( form.method, form.action, true );
	formRequest.setRequestHeader( 'Content-type','application/x-www-form-urlencoded' );
	formRequest.send( finalString );

	formRequest.onreadystatechange = function(){
		if( formRequest.readyState == 4 && formRequest.status == 200 ){
			if( verbose ) viewBoxObj.innerHTML = formRequest.responseText;
		}
	}	

	return false;
}

//-------------------------------------------------------------------------------------------------------
function uploadForm( form, viewBoxID )
{
	var viewBoxObj = document.getElementById( viewBoxID );
	if( viewBoxObj == undefined ){
		viewBoxObj = document.getElementById( formViewBoxID );
		if( viewBoxObj == undefined ){
			console.log( "form.js: uploadForm: viewBoxObj not found" );
			return;
		}
	}

	viewBoxObj.innerHTML = '<img src="/data/images/loader.gif">';

	var formData = new FormData( form );
	formRequest.open( "POST", form.action, false );
	formRequest.send( formData );

	viewBoxObj.innerHTML = formRequest.responseText;

	return false;
}

function sendForm( form )
{
	var formData = new FormData( form );
	formRequest.open( "POST", form.action, false );
	formRequest.send( formData );

	return JSON.parse( ( formRequest.responseText != '' ) ? formRequest.responseText : '{}' );
}

//-------------------------------------------------------------------------------------------------------
//<input type="tel" name="data[phone]" value="+7(___)___-__-__" onInput="phoneMask( this );">
function phoneMask( input )
{
	let patStringArr = "+7(___)___-__-__".split('');
	let arrPush = [3, 4, 5, 7, 8, 9, 11, 12, 14, 15]
	let val = input.value;
	let arr = val.replace(/\D+/g, "").split('').splice(1);
	let n;
	let ni;
	arr.forEach((s, i) => {
		n = arrPush[i];
		patStringArr[n] = s
		ni = i
	});
	arr.length < 10 ? input.style.color = 'red' : input.style.color = 'green';
	input.value = patStringArr.join('');
	n ? v.setSelectionRange(n + 1, n + 1) : input.setSelectionRange(17, 17)
}

//-------------------------------------------------------------------------------------------------------
function checkMinMaxfield( field )
{
	let min = Number( field.min );
	let max = Number( field.max );

	if( field.value != '*' ){
		let value = Number( field.value );
		if( value < min ){
			field.value = min;
		}else if( value > max ){
			field.value = max;
		}
	}
}

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
/////////////////////////////////	COLOR		/////////////////////////////////////////////////////////

function hexToRgb( hex )
{
	hex = parseInt(((hex.indexOf('#') > -1) ? hex.substring(1) : hex), 16);
	return {r: hex >> 16, g: (hex & 0x00FF00) >> 8, b: (hex & 0x0000FF)};
}

function hexToHsb( hex )
{
	return rgbToHsb( hexToRgb( hex ) );
}

function rgbToHsb( rgb )
{
	var hsb = {h: 0, s: 0, b: 0};
	var min = Math.min(rgb.r, rgb.g, rgb.b);
	var max = Math.max(rgb.r, rgb.g, rgb.b);
	var delta = max - min;
	hsb.b = max;
	hsb.s = max != 0 ? 255 * delta / max : 0;
	if (hsb.s != 0) {
		if (rgb.r == max) hsb.h = (rgb.g - rgb.b) / delta;
		else if (rgb.g == max) hsb.h = 2 + (rgb.b - rgb.r) / delta;
		else hsb.h = 4 + (rgb.r - rgb.g) / delta;
	}else hsb.h = -1;
	hsb.h *= 60;
	if (hsb.h < 0) hsb.h += 360;
	hsb.s *= 100 / 255;
	hsb.b *= colpicData.brightMax / 255;
	return hsb;
}

function hsbToRgb( hsb )
{
	var rgb = {};
	var h = hsb.h;
	var s = hsb.s * 255 / 100;
	var v = hsb.b * 255 / brightMax;
	if (s == 0) {
		rgb.r = rgb.g = rgb.b = v;
	} else {
		var t1 = v;
		var t2 = (255 - s) * v / 255;
		var t3 = (t1 - t2) * (h % 60) / 60;
		if (h == 360) h = 0;
		if (h < 60) {
			rgb.r = t1;
			rgb.b = t2;
			rgb.g = t2 + t3
		}
		else if (h < 120) {
			rgb.g = t1;
			rgb.b = t2;
			rgb.r = t1 - t3
		}
		else if (h < 180) {
			rgb.g = t1;
			rgb.r = t2;
			rgb.b = t2 + t3
		}
		else if (h < 240) {
			rgb.b = t1;
			rgb.r = t2;
			rgb.g = t1 - t3
		}
		else if (h < 300) {
			rgb.b = t1;
			rgb.g = t2;
			rgb.r = t2 + t3
		}
		else if (h < 360) {
			rgb.r = t1;
			rgb.g = t2;
			rgb.b = t1 - t3
		}
		else {
			rgb.r = 0;
			rgb.g = 0;
			rgb.b = 0
		}
	}
	return {r: Math.round(rgb.r), g: Math.round(rgb.g), b: Math.round(rgb.b)};
}

function rgbToHex( rgb )
{
	var hex = [
		rgb.r.toString(16),
		rgb.g.toString(16),
		rgb.b.toString(16)
	];

	if( hex[0].length == 1 ) hex[0] = '0' + hex[0];
	if( hex[1].length == 1 ) hex[1] = '0' + hex[1];
	if( hex[2].length == 1 ) hex[2] = '0' + hex[2];

	return hex.join('');
}

function hsbToHex( hsb )
{
	return rgbToHex( hsbToRgb( hsb ) );
}