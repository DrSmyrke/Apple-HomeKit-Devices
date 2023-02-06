var formRequest					= makeHttpObject();
var formViewBoxID				= "";
var brightMax					= 100;

//-------------------------------------------------------------------------------------------------------
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
function sendForm( form )
{
	var formData = new FormData( form );
	formRequest.open( "POST", form.action, false );
	formRequest.send( formData );

	return JSON.parse( ( formRequest.responseText != '' ) ? formRequest.responseText : '{}' );
}
