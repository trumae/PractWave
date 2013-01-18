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
