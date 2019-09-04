class TemplateRepository
{
    constructor()
    {
        this.templates = {};
    }

    /**
     * @param {string} name
     * @param {function} callback
     */
    load(name, callback)
    {
        let repository = this;

        if (this.templates[name] !== undefined) {
            callback(this.templates[name]);
            return;
        }

        $.get( "/templates/" + name + ".html", function( data ) {
            Mustache.parse(data);
            repository.templates[name] = data;

            callback(data);
        });
    }
}