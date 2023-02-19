---
layout: post
title: Installation
---
To install PBDoom on your PocketBook device do the following:

1. Download the game archive. The latest version can be downloaded here:
{%- capture bc -%}
  Downlod PBDoom ({{ site.data.releases[0].name }})
{%- endcapture -%}
{%- assign href  = site.data.releases[0].download_url -%}
<span class="not-prose">{%- include button.html
  href=href
  icon="heroicons/solid/document_arrow_down.svg"
  text=bc
  class="m-0"
-%}</span>

2. Unzip the archive to the `applications` directory on the ereader.

3. Place your WADs in the `applications/pbdoom` directory. WAD filenames should
   be **all lower case** (not `DOOM2.WAD` but `doom2.wad`)

4. You should have this structure:
```
applications
├── pbdoom
│   ├── doom2.wad
│   ├── game.app
└── pbdoom.app
```

5. Run the game using the `pbdoom` icon from the ereader applications menu
