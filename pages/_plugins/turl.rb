module Jekyll
  module Turl
    def turl(ref)
      site = @context.registers[:site]
      lang = @context.registers[:page]['lang']

      page = (site.pages + site.posts.docs)
        .find { |x| x.data['ref'] == ref && x.data['lang'] == lang }

      raise "No translation to #{lang} for ref #{ref}" unless page

      page.url
    end

    def lang_self(lang)
      site = @context.registers[:site]
      ref = @context.registers[:page]['ref']

      page = (site.pages + site.documents)
        .find { |x| x.data['ref'] == ref && x.data['lang'] == lang }

      unless page
        raise <<~ERR
          No translation to #{lang} for ref '#{ref.inspect}'
          on page #{@context.registers[:page]['path']}
        ERR
      end

      page.url
    end
  end
end

Liquid::Template.register_filter(Jekyll::Turl)
