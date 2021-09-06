#!/usr/bin/env python

"""
Copyright (c) 2017-2021 ARM Limited. All rights reserved.
SPDX-License-Identifier: Apache-2.0
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations 
"""
import click
import requests
import logging
import sys
import time
import json
import subprocess

"""
This file contains ghcr utlity wrapper used for:
- retrieving digest of docker image
- deleting images in ghcr
"""


@click.command()
@click.pass_context
@click.option("-r", "--repository", required=True)
@click.option("-t", "--tag", required=True)
@click.option("-p", "--platform", required=False)
def get_digest(ctx, repository, tag, platform=None):
    """
    Prints docker digest of specific platform of multi architecture image
    :param ctx: click context
    :param repository: docker repository
    :param tag: docker tag
    :param platform: platform for e.g, linux/arm64
    """
    command = f"docker run quay.io/skopeo/stable:v1.4.1 inspect --creds={ctx.obj['username']}:{ctx.obj['passwd']} docker://ghcr.io/{ctx.obj['username']}/{repository}:{tag} --raw"
    output = subprocess.run(
        command.split(), stdout=subprocess.PIPE, check=True
    ).stdout.decode("utf-8")
    output = json.loads(output)

    images = output["manifests"]
    digest = ""
    if len(images) and platform is None:
        logging.error(
            "This tag has more than one platform associated to it, please input a platform"
        )
        sys.exit(1)

    for image in images:
        if platform != None:
            if (platform.split("/")[0] == image["platform"]["os"]) and (
                platform.split("/")[1] == image["platform"]["architecture"]
            ):
                digest = image["digest"]
        else:
            digest = image["digest"]

    if digest == "":
        logging.error("Digest not found. image not in repo for the given platform")
        sys.exit(1)

    print(digest)


@click.command()
@click.pass_context
@click.option("-r", "--repository", required=True)
@click.option(
    "-n",
    "--number_of_days",
    default="10",
    help="number of days since image was created",
    required=False,
)
def delete_old_images(ctx, repository, number_of_days):
    """
    delete old images from docker repository

    :param ctx: click context
    :param repository: docker repository
    :param number_of_days: delete older than these number of days
    """
    with requests.Session() as s:
        github_api_accept = "application/vnd.github.v3+json"
        s.headers.update(
            {"Authorization": f'token {ctx.obj["passwd"]}', "Accept": github_api_accept}
        )
        r = s.get(
            f"https://api.github.com/user/packages/container/{repository}/versions"
        )
        versions = r.json()
        version_id = None
        pattern = "%d.%m.%Y %H:%M:%S"
        pattern = "%Y-%m-%dT%H:%M:%SZ"
        current_time = time.time()
        for version in versions:
            logging.info(version)
            epoch = int(time.mktime(time.strptime(version["updated_at"], pattern)))

            if (current_time - epoch) / (24 * 60 * 60) > int(number_of_days):
                version_id = version["id"]
                logging.debug(f"deleteing image with version id {version_id}")

                url = f"https://api.github.com/user/packages/container/{repository}/versions/{version_id}"
                resp = s.delete(url)
                resp.raise_for_status()


@click.group()
@click.pass_context
@click.option("-u", "--username", required=False)
@click.option("-p", "--passwd", required=False)
@click.option("-v", "--verbose", is_flag=True, default=False)
def main(ctx, username, passwd, verbose):
    ctx.obj = {"username": username, "passwd": passwd}

    if verbose:
        logging.basicConfig(
            stream=sys.stdout,
            format="%(levelname)s %(asctime)s %(message)s",
            datefmt="%m/%d/%Y %I:%M:%S %p",
        )
        logging.getLogger().setLevel(logging.DEBUG)
    else:
        logging.basicConfig(
            format="%(levelname)s %(asctime)s %(message)s",
            datefmt="%m/%d/%Y %I:%M:%S %p",
        )
        logging.getLogger().setLevel(logging.INFO)


if __name__ == "__main__":
    main.add_command(get_digest)
    main.add_command(delete_old_images)
    main()
