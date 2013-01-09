function initCalculadora(){
	var htmlCalculadora = '<div class="grid">'+
						'	<div class="row">'+
						'		<div id="res" name="res" class="display span4"></div>'+
						'	</div>'+
						'	<div class="row">'+
						'		<div class="span1 botaoCalc" onclick="porcento()">%</div>'+
						'		<div class="span1 botaoCalc" onclick="sinal(\' v\');">&radic;&macr;</div>'+
						'		<div class="span1 botaoCalc" onclick="sinal(\' Log\');">Log</div>'+
						'		<div class="span1 botaoCalc" onclick="sinal(\' ^\');">X&sup2;</div>'+
						'	</div>'+
						'	<div class="row">'+
						'		<div class="span1 botaoCalc" onclick="calc(\'7\');">7</div>'+
						'		<div class="span1 botaoCalc" onclick="calc(\'8\');">8</div>'+
						'		<div class="span1 botaoCalc" onclick="calc(\'9\');">9</div>'+
						'		<div class="span1 botaoCalc" onclick="sinal(\' &divide;\');">&divide</div>'+
						'	</div>'+
						'	<div class="row">'+
						'		<div class="span1 botaoCalc" onclick="calc(\'4\');">4</div>'+
						'		<div class="span1 botaoCalc" onclick="calc(\'5\');">5</div>'+
						'		<div class="span1 botaoCalc" onclick="calc(\'6\');">6</div>'+
						'		<div class="span1 botaoCalc" onclick="sinal(\' X\');">X</div>'+
						'	</div>'+
						'	<div class="row">'+
						'		<div class="span1 botaoCalc" onclick="calc(\'1\');">1</div>'+
						'		<div class="span1 botaoCalc" onclick="calc(\'2\');">2</div>'+
						'		<div class="span1 botaoCalc" onclick="calc(\'3\');">3</div>'+
						'		<div class="span1 botaoCalc" onclick="sinal(\' -\');">-</div>'+
						'	</div>'+
						'	<div class="row">'+
						'		<div class="span1 botaoCalc" onclick="limpa()">CE</div>'+
						'		<div class="span1 botaoCalc" onclick="calc(\'0\');">0</div>'+
						'		<div class="span1 botaoCalc" onclick="calcula()">=</div>'+
						'		<div class="span1 botaoCalc" onclick="sinal(\' +\');">+</div>'+
						'	</div>'+
						'	<div class="row">'+
						'		<div class="span1 botaoCalc" onclick="calc(\'.\');">.</div>'+
						'	</div>'+
						'</div>';
	$("#conteudoCalculadora").append(htmlCalculadora);
}

function calc(valor) {
	$("#res").append(valor);
}
																        
function sinal(oper) {
	$("#res").append(oper + " ");
}
																														        
function calcula() {
    vet = $("#res").html();
    vet2 = vet.split(' ');
    switch(vet2[1]) {
		case 'X':
			resultado = vet2[0] * vet2[2];
			break;
		case '÷':
		    resultado = vet2[0] / vet2[2];
		    break;
		case '+':
		    resultado = parseFloat(vet2[0]) + parseFloat(vet2[2]);
		    break;
		case '-':
		    resultado = vet2[0] - vet2[2];
			break;
		case "v":
		    resultado = Math.sqrt(vet2[0]);
		    break;
		case 'Log':
		    resultado = Math.LOG10E*Math.log(vet2[0]);
		    break;
		case '^':
		    resultado = Math.pow(vet2[0], vet2[2]);
		    break;
	}
	
	$('#res').html(resultado);
	return true;
}

function limpa() {
	$('#res').html("");
}

function porcento() {
	vet = $('#res').html();
	vet2 = vet.split(' ');
	if(vet2[1] == 'X')
		$('#res').html((vet2[0] * vet2[2])/100);
	else if(vet2[1] == '+')
		$('#res').html(parseFloat((vet2[0] * vet2[2])/100) + parseFloat(vet2[0]));
	else if(vet2[1] == '-')
		$('#res').html(parseFloat(vet2[0]) - parseFloat((vet2[0] * vet2[2])/100));
}
