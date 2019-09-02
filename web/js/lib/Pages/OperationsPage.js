class OperationsPage
{
    constructor()
    {
        this.speedSlider = undefined;
    }

    start()
    {
        this.speedSlider = $('.control.segment .slider')
            .slider({
                min: 0,
                max: 7,
                start: 2,
                step: 0.5
            })
        ;
    }
}