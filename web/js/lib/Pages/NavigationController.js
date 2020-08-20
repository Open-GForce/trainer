class NavigationController
{
    constructor()
    {
        /**
         * @type {jQuery}
         */
        this.navigationElement = undefined;

        /**
         * @type {AbstractPage[]}
         */
        this.pages = [];
    }

    start()
    {
        this.navigationElement = $('#navigation');

        this.pages = [
            new OperationsPage(),
            new SettingsPage()
        ];

        this.navigationElement.find('a.item').each(function (index) {
            let item = $(this);

            item.click(function () {
                app.navigation.navigationElement.find('.item').removeClass('active');
                item.addClass('active');

                app.currentPage.shutdown();
                app.currentPage = app.navigation.pages[index];
                app.currentPage.start();
            });
        });

        app.currentPage = this.pages[0];
        app.currentPage.start();
    }


}