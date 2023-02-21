module Jekyll
  module T
    def t(obj)
      raise "Can't translate nil" unless obj

      lang = @context.registers[:page]['lang']

      translation = obj[lang]

      unless translation
        raise <<~MSG
        No translation for lang #{lang} in #{obj} on page #{@context.registers[:page]['path']}
        MSG
      end

      translation
    end

    def tdate(obj)
      unless obj
        raise <<~MSG
          Can't localize nil date on page #{@context.registers[:page]['path']}
        MSG
      end

      obj = Time.parse(obj) if obj.is_a?(String)

      months = @context.registers[:site].data['i18n']['months']

      "#{obj.day} #{t(months[obj.month - 1])} #{obj.year}"
    end
  end
end

Liquid::Template.register_filter(Jekyll::T)
