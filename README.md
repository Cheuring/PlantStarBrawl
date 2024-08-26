# PlantStarBrawl
## Brief introduction
A project for the course "Programming Practice".

Mainly inspired by [***@Voidmatrix***](https://www.bilibili.com/video/BV1jx4y1t7eP/?spm_id_from=333.788&vd_source=fe11182595b7da452c0a2309055a8004), adding two characters and brand new buff game mechanics. Additionally, there is a beta online version.

## GetStart
first you need to install [**easyx**](https://easyx.cn/) and configure it correctly

then you can clone this project and run

```shell
mkdir build
cd build
cmake ..
make .
```
and you will see the server , client and local version of the game

#### operation (online)
|  Key  | Action|
| ----- | ----- |
| **W** | jump  |
| **S** | drow  |
| **A** | left  |
| **D** | right |
| **J** | attack|
| **K** | ex_attack|

#### operation (local)

| Player 1  | Action    | Player 2  | Action    |
| ----- | -----     | ----- | -----     |
| **W** | jump      | **↑** | jump      |
| **S** | drow      | **↓** | drow      |
| **A** | left      | **←** | left      |
| **D** | right     | **→** | right     |
| **F** | attack    | **.** | attack    |
| **G** | ex_attack | **/** | ex_attack |


## TODO

- add lockstep
- add fancy UI when connecting
- add more characters
- add more buff
- add more game mechanics

## License

MIT
