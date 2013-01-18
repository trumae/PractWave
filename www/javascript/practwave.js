function ativaTilesSlider () {
    var slidedTiles = $('[data-role=tile-slider], .block-slider, .tile-slider');
    slidedTiles.each(function (index, tile) {
        var params = {};
        tile = $(tile);
        params.direction = tile.data('paramDirection');
        params.duration = tile.data('paramDuration');
        params.period = tile.data('paramPeriod');
        tile.tileBlockSlider(params);
    });
}

function initMainMenu() {
    ativaTilesSlider();
}

function ativaCarousel(){
    var allCarousels = $('[data-role=carousel], .carousel');
    allCarousels.each(function (index, carousel) {
        var params = {};
        $carousel = $(carousel);
        params.auto         = $carousel.data('paramAuto');
        params.period       = $carousel.data('paramPeriod');
        params.duration     = $carousel.data('paramDuration');
        params.effect       = $carousel.data('paramEffect');
        params.direction    = $carousel.data('paramDirection');
        params.markers      = $carousel.data('paramMarkers');
        params.arrows       = $carousel.data('paramArrows');
        params.stop         = $carousel.data('paramStop');

        $carousel.Carousel(params);
    })
}
