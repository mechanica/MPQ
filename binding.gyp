{
  "targets": [
    {
      "target_name": "mech_mpq",
      "sources": [
        "src/mpqtools.cc",
        "src/mpqarchive.cc",
        "src/mpqfile.cc"
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
