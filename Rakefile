require 'uri'
require 'net/http'
require 'json'
require 'yaml'
require 'date'

desc 'Update releases data file'
task :releases do
  uri = URI('https://api.github.com/repos/imustafin/pbdoom/releases')
  res = Net::HTTP.get_response(uri)
  unless res.is_a?(Net::HTTPSuccess)
    raise "Error getting releases #{res}"
  end

  releases = JSON.parse(res.body)

  # result should have string keys
  result = releases.map do |r|
    applications = r['assets'].find { |x| x['name'] == 'applications.zip' }

    {
      'html_url' => r['html_url'],
      'name' => r['name'],
      'published_at' => r['published_at'],
      'download_url' => applications['browser_download_url']
    }
  end

  File.write('_data/releases.yml', result.to_yaml)
end
