function initCalculadora(){
	var htmlCalculadora = '<div class="grid">'+
						'	<div class="row">'+
						'		<div id="res" name="res" class="display span8 bg-color-blue-light"></div>'+
						'	</div>'+
						'	<div class="row">'+
						'		<div class="span2 botaoCalc bg-color-orange" onclick="porcento()">%</div>'+
						'		<div class="span2 botaoCalc bg-color-orange" onclick="sinal(\' v\');">&radic;&macr;</div>'+
						'		<div class="span2 botaoCalc bg-color-orange" onclick="sinal(\' Log\');">Log</div>'+
						'		<div class="span2 botaoCalc bg-color-orange" onclick="sinal(\' ^\');">X&sup2;</div>'+
						'	</div>'+
						'	<div class="row">'+
						'		<div class="span2 botaoCalc bg-color-blue" onclick="calc(\'7\');">7</div>'+
						'		<div class="span2 botaoCalc bg-color-blue" onclick="calc(\'8\');">8</div>'+
						'		<div class="span2 botaoCalc bg-color-blue" onclick="calc(\'9\');">9</div>'+
						'		<div class="span2 botaoCalc bg-color-orange" onclick="sinal(\' &divide;\');">&divide</div>'+
						'	</div>'+
						'	<div class="row">'+
						'		<div class="span2 botaoCalc bg-color-blue" onclick="calc(\'4\');">4</div>'+
						'		<div class="span2 botaoCalc bg-color-blue" onclick="calc(\'5\');">5</div>'+
						'		<div class="span2 botaoCalc bg-color-blue" onclick="calc(\'6\');">6</div>'+
						'		<div class="span2 botaoCalc bg-color-orange" onclick="sinal(\' x\');">x</div>'+
						'	</div>'+
						'	<div class="row">'+
						'		<div class="span2 botaoCalc bg-color-blue" onclick="calc(\'1\');">1</div>'+
						'		<div class="span2 botaoCalc bg-color-blue" onclick="calc(\'2\');">2</div>'+
						'		<div class="span2 botaoCalc bg-color-blue" onclick="calc(\'3\');">3</div>'+
						'		<div class="span2 botaoCalc bg-color-orange" onclick="sinal(\' -\');">-</div>'+
						'	</div>'+
						'	<div class="row">'+
						'		<div class="span2 botaoCalc bg-color-blue" onclick="calc(\'.\');">.</div>'+
						'		<div class="span2 botaoCalc bg-color-blue" onclick="calc(\'0\');">0</div>'+
						'		<div class="span2 botaoCalc bg-color-blue" onclick="limpaUm()">&laquo;</div>'+
						'		<div class="span2 botaoCalc bg-color-orange" onclick="sinal(\' +\');">+</div>'+
						'	</div>'+
						'	<div class="row">'+
						'		<div class="span4 botaoGrande bg-color-orangeDark" onclick="limpa()">CE</div>'+
						'		<div class="span4 botaoGrande bg-color-greenLight" onclick="calcula()">=</div>'+
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
	var resultado;
    var vet = $("#res").html();
	vet = vet.replace(/^\s+|\s+$/g,"");
    var vet2 = vet.split(' ');
	if(!operacaoOK(vet2))
		return false;
    switch(vet2[1]) {
		case 'x':
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
	var vet = $('#res').html();
	var vet2 = vet.split(' ');
	if(vet2[1] == 'x')
		$('#res').html((vet2[0] * vet2[2])/100);
	else if(vet2[1] == '+')
		$('#res').html(parseFloat((vet2[0] * vet2[2])/100) + parseFloat(vet2[0]));
	else if(vet2[1] == '-')
		$('#res').html(parseFloat(vet2[0]) - parseFloat((vet2[0] * vet2[2])/100));
}

function operacaoOK(vet2){
	if(vet2.length > 3 || vet2.length <=1)
		return false;

	if(vet2.length == 2){
		if(!isOperacao(vet2[0]) && isOperacao(vet2[1]))
			return true;
		else
			return false;
	}else{
		if(!isOperacao(vet2[0]) && isOperacao(vet2[1]) && !isOperacao(vet2[2]))
			return true;
		else
			return false;
	}
}

function isOperacao(x){
    var is = false;
	switch(x) {
		case 'x':
		case '÷':
		case '+':
		case '-':
		case "v":
		case 'Log':
		case '^':
			is = true;
		    break;
	}
	return is;
}

function limpaUm(){
    var vet = $("#res").html();
	vet = vet.replace(/^\s+|\s+$/g,"");
    var vet2 = vet.split(' ');
	if(vet2.length == 0)
		return;
	var ret = '';
	for(var i=0;i<(vet2.length - 1);i++){
		ret+=vet2[i];
		if(i<(vet2.length - 2))
			ret+=" "
	}
	$("#res").html(ret);
}
