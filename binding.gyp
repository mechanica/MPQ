{
  "targets": [
    {
      "target_name": "mpqtools",
      "sources": [
        "src/mpqtools.cc",
        "src/mpqtarchive.cc",
        "src/mpqtfile.cc"
      ],
      "link_settings": {
        "libraries": [
          "../src/StormLib/storm.framework/storm"
        ]
      },
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
