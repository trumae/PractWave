function initLoja(){
    ativaCarousel();
	$(".conteudoApp").hide();
	
	$(".goLoja").click(function(){
		$(".conteudoApp").hide();
		$("#Loja").show();
	});

	$("#TileCalculadora").click(function(){
		$(".conteudo").hide();
		$("#AppCalculadora").show();
	});
}
