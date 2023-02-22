window.onload = function(){
	formRequest.open( 'GET', '/get', true );
	formRequest.send( null );

	formRequest.onreadystatechange = function(){
		if( formRequest.readyState == 4 && formRequest.status == 200 ){
			let dataObject = JSON.parse( ( formRequest.responseText != '' ) ? formRequest.responseText : '{}' );
			if( dataObject.hasOwnProperty( "on" ) ){
				document.forms.onoff.show.checked = ( dataObject.on ) ? true : false;
				document.forms.onoff.onoff.value = ( dataObject.on ) ? 1 : 0;
			}
			if( dataObject.hasOwnProperty( "hue" ) ){
				document.forms.hsb.hue.value = dataObject.hue;
			}
			if( dataObject.hasOwnProperty( "sat" ) ){
				document.forms.hsb.sat.value = dataObject.sat;
			}
			if( dataObject.hasOwnProperty( "bri" ) ){
				document.forms.hsb.bri.value = dataObject.bri;
			}
		}
	}
};

var timeout = 0;
function updateT()
{
	if( timeout > 0 ){
		timeout--;
		setTimeout( updateT, 10 );
	}else{
		let form = document.forms.hsb;
		
		let formData = new FormData( form );
		formRequest.open( "POST", form.action, false );
		formRequest.send( formData );
	}
}

function updateHSB()
{
	if( timeout <= 0 ){
		timeout = 25;
		updateT();
	}else{
		timeout = 25;
	}
}

function dev_action( value )
{
	let form = document.forms.onoff;

	form.onoff.value = ( value == true ) ? 1 : 0;
	
	let formData = new FormData( form );
	formRequest.open( "POST", form.action, false );
	formRequest.send( formData );
}