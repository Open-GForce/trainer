class OperationsPage
{
    constructor()
    {
        /**
         * @type {*}
         */
        this.speedSlider = undefined;

        /**
         * @type {*}
         */
        this.brakeInputCanvas = undefined;

        /**
         * @type {Chart}
         */
        this.brakeInputChart = undefined;

        /**
         * @type {*}
         */
        this.speedCanvs = undefined;

        /**
         * @type {Chart}
         */
        this.speedChart = undefined;

        /**
         * @type {{legend: {display: boolean}, responsive: boolean, scales: {yAxes: {ticks: {max: number, beginAtZero: boolean}}[]}}}
         */
        this.baseChartOptions = {
            responsive: false,
            legend: {
                display: false
            },
            scales: {
                yAxes: [{
                    ticks: {
                        beginAtZero: true,
                        max: 100,
                    }
                }]
            }
        }
    }

    start()
    {
        this.speedSlider = $('.control.segment .slider').slider({
                min: 0,
                max: 7,
                start: 2,
                step: 0.5
            });

        this.brakeInputCanvas = $('#brakeInputChart');
        this.brakeInputChart = new Chart(this.brakeInputCanvas, {
            type: 'bar',
            data: {
                labels: ['Außen', 'Innen', 'Gesamt'],
                datasets: [{
                    label: 'Bremswert',
                    data: [25, 80, 65],
                    backgroundColor: [
                        '#b71c1c',
                        '#689f38',
                        '#1976d2'
                    ],
                    borderWidth: 1
                }]
            },
            options: this.baseChartOptions
        });

        this.speedCanvs = $('#speedChart');
        let speedChartOptions = this.baseChartOptions;
        speedChartOptions.scales.yAxes[0].ticks.max = 3500;

        this.brakeInputChart = new Chart(this.speedCanvs, {
            type: 'bar',
            data: {
                labels: ['Ziel', 'Aktuell', 'Max'],
                datasets: [{
                    label: 'Bremswert',
                    data: [3214, 2800, 3400],
                    backgroundColor: [
                        '#1976d2',
                        '#63a4ff',
                        '#004ba0'
                    ],
                    borderWidth: 1
                }]
            },
            options: speedChartOptions
        });
    }
}