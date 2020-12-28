## Importing repositories into mbed-os

importer.py script can be used to import code base from different repositories into mbed-os.

### Pre-requisties
1. Get the required repository clone and update it to commit required - the recommended would be to checkout the latest release version in the repository that we are getting an update from.
Note: Repository should be placed outside the mbed-os. For instance [CMSIS_5](https://github.com/ARM-software/CMSIS_5) repository would be at the same level as mbed-os repository (`update/mbed-os` and update/CMSIS_5)
2. Create json file as per template

### JSON file template

You can list all the files and folders which are required to be copied in config file in `file` and `folder` arrays respectively. Script will remove the files/folders specified in `src` section from mbed-os repository before copy operation is performed.
New commit is created on branch `feature_+repo_name+last_sha` with commit message "[REPO_NAME]: Updated to last_sha"

Note: Only files present in folder will be copied, directories inside the folder will not be copied.

`commit_sha` is list of commits present in mbed-os repo. They are used to be on top of the update - we are changing the upstream repository. To illustrate, the upstream repository does not contain Mbed OS specific change. To apply this change for every update, we add this commit SHA to the `commit_sha`. Every update we do, will use this sha and apply it on top of the latest update. Each commit in the commit_sha list is cherry-picked and applied with the -x option, which records the SHA of the source commit in the commit message.
Note: You must resolve any conflicts that arise during this cherry-pick process. Make sure that the "(cherry picked from commit ...)" statement is present in the commit message. After the conflict resolution, continue with the cherry pick. Once done, just re-execute the python script to apply rest of the SHA commits.

```json
{
  "files" : [
    {
      "src_file" : "CMSIS/Core/Template/ARMv8-M/tz_context.c",
      "dest_file" : "cmsis/CMSIS_5/CMSIS/TARGET_CORTEX_M/Source/mbed_tz_context.c"
    },
    ...
    {
      "src_file" : "",
      "dest_file" : ""
    }
  ],
  "folders" : [
    {
      "src_folder" : "CMSIS/Core/Include/",
      "dest_folder" : "cmsis/CMSIS_5/CMSIS/TARGET_CORTEX_M/Include"
    },
    ...
    {
      "src_folder" : "",
      "dest_folder" : ""
    }
  ],
   "commit_sha" : [
      "428acae1b2ac15c3ad523e8d40755a9301220822",
      {"sha": "d9d622afe0ca8c7ab9d24c17f9fe59b54dcc61c9", "msg": "Fix ..."}
    ]
}
```

### Input to importer.py
1. Repository: -r <repo_path> ( Example: CMSIS / Mbed-tls)
2. `repo`_importer.json: -c <json_file>  (Example: cmsis_importer.json)

For example the command below can be used to update CMSIS:
`python tools\importer\importer.py -c tools\importer\cmsis_importer.json -r <path to cmsis repo>`

