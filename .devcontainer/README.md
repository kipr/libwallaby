# libwallaby devcontainer

A Debian 13 environment that builds libwallaby for the Wombat (`aarch64`) and for the desktop (`-DDUMMY=ON`), with the Claude Code and Codex CLIs installed.

## What's in the image

The image is the `dev` stage of the repo's root [`Dockerfile`](../Dockerfile):

- **`toolchain` stage** (shared with the release builder): the `aarch64` cross-compiler, CMake, SWIG, Doxygen, and the `arm64` X11 and Python 3.13 libraries the Wombat build links against.
- **`dev` stage** adds:
  - the native X11 and Python 3.13 libraries for the desktop build
  - dev tools: fish, git, ripgrep, fd, fzf, jq, curl, ssh
  - the Claude Code and Codex CLIs (Codex needs Node, so `nodejs`/`npm` are installed too)
  - bubblewrap for the agents' sandbox

The user is `code` (UID 1000, home `/home/code`, shell fish).

The `builder` stage, which `docker build .` builds by default, is the release builder from the main README. Its toolchain comes from the same `toolchain` stage, so a toolchain change applies to both.

## Opening it

- VS Code: run **Dev Containers: Reopen in Container**.
- CLI: `devcontainer up --workspace-folder .`, then `devcontainer exec --workspace-folder . fish`.

To build the image without the devcontainer tooling:

```sh
docker build --target dev -t libwallaby-dev .
```

Rebuild the container after changing the `Dockerfile` or `devcontainer.json`.

## Building libwallaby inside it

Wombat (`aarch64`), which also produces `build/kipr-<version>-Linux.deb`:

```sh
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=toolchain/aarch64-linux-gnu.cmake .
cmake --build build -j "$(nproc)"
```

Desktop (dummy backend):

```sh
cmake -Bbuild-desktop -DDUMMY=ON -Dpackage_debian=OFF -Dwith_camera=OFF .
cmake --build build-desktop -j "$(nproc)"
```

The camera module downloads its dependencies, so the Wombat build needs internet access.
The Simulator (Emscripten) build is not set up here. It needs the emsdk, which is not in the image.

## Agent CLIs

Claude Code and Codex keep their logins and settings in Docker named volumes, `claude-config` and `codex-config`.
Every devcontainer that uses these volume names shares them, so you log in once per machine.
The volumes survive container rebuilds. Remove them with `docker volume rm claude-config codex-config`.

First-time login and smoke test:

```sh
claude          # follow the login prompt, then /exit
claude -p "say hi"
codex login
codex exec "say hi"
codex sandbox -- true   # checks the bubblewrap sandbox
```

Containers share the volumes live, so agent sessions running in two containers at once write to the same files.

Host-level agent config (user settings, skills, plugins, memories) is not shared with the container. Project-level config (`.claude/`, `AGENTS.md`) comes with the workspace.

## Caveats

- **seccomp and path masking are disabled.** The agents' bubblewrap sandbox fails under Docker's default seccomp profile and can't mount `/proc` while Docker masks paths under it. The container therefore runs with `--security-opt=seccomp=unconfined` and `--security-opt=systempaths=unconfined`, which turn off Docker's syscall filter and its `/proc`/`/sys` masking for the container.
- **SELinux hosts.** If the workspace mount is denied (for example under Podman), add `"--security-opt=label=disable"` to `runArgs`.
- **No hardware access.** The container can't reach a Wombat over USB. Copy the `.deb` to the Wombat over the network as the main README describes.
