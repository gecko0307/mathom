3. Model Loading
================
DGL provides a convenient way to load 3D models from files. The file format for this is DGL3, an easy to read and write binary format (read dgl3-specification.txt to learn more about it). We provide Blender exporter for DGL3, so you can use Blender as an asset creation tool for your games - install `io_export_dgl3.py` from `tools` folder.

Creating the asset
------------------
Let's start with an empty Blender scene without any objects (select all, then delete). Add Monkey object.

Exporting
---------
Select File -> Export -> DGL Scene (.dgl3) and save your asset somewhere - for example, in `media` folder inside your game directory. All object names will be kept, so you can later access them in the game.

Materials
---------
Material export from Blender is currently WIP status. Nevertheless, you can create materials manually. DGL would search material files (and texture files) in its mounted directories. Material files are plain text files with *.mat extension and their names should be the same as in Blender (excluding the extension). For example, if material name is `myMaterial` then its filename would be `myMaterial.mat`. Objects without material will be exported with default material assigned. Also DGL assigns default material to entities if it fails to find their materials in mounted directories.

Typical material file looks as follows:

    name: "myMaterial";
    ambientColor: [0.3, 0.4, 0.5, 1.0];
    diffuseColor: [1.0, 1.0, 1.0, 1.0];
    specularColor: [1.0, 1.0, 1.0, 1.0];
    emissionColor: [0.0, 0.0, 0.0, 1.0];
    roughness: 0.5;
    specularity: 0.7;
    diffuseTexture: "diffuse.png";
    normalTexture: "normal.png";
    
Color and texture properties are self-descriptive, `roughness` property defines 'blurriness' of the specular highlight (shiny materials have low roughness), `specularity` defines brightness of the specular highlight.

DGL treats normal texture's alpha channel as heightmap for parallax mapping. If you don't want to use parallax mapping, remove the alpha channel (make RGB file instead of RGBA).

Currently DGL supports only PNG files for textures.

Loading
-------
DGL offers a threaded procedure of asset loading. For `Application3D` it looks as simple as follows:

```d
setDefaultLoadingImage("media/loading.png");
mountDirectory("media");
string modelFile = "suzanne.dgl3";
auto model = addModelResource(modelFile);
loadResources();
```

`media/loading.png` is an image that should be drawn while assets are loaded. You can override default loading screen with your own rendering code, we will surely return to this feature later.

`mountDirectory` defines a local directory where the asset manager should look for *.dgl3 files - in this example, it is `media`. You can mount several directories - they will be searched for assets from first one to last one (first has highest priority).

`addModelResource` adds a *.dgl3 file to the list of assets that should be loaded. Actual loading is done by `loadResources`.

Now when asset is loaded, we can add its entities to the scene: 

```d
foreach(e; model.entities)
{
    addEntity3D(e);
    if (e.material)
        e.material.setShader();
}
```

If GLSL shaders are supported, `setShader` method will assign uber shader to the material. This shader handles normal mapping, shadows and other rendering features.

You would also want to add a light source:

```d
addPointLight(Vector3f(0, 5, 5));
```

Viola, you now have your Blender scene in DGL!

Deleting
--------
If you want to load a new scene (e.g., when starting a new level), you may want to delete the old one. This is very easy: just call `freeResources` (it will delete all resources and current entities and lights in a scene), add new model resource and call `loadResources` once again:

```d
freeResources();
model = addModelResource("yourAnotherScene.dgl3");
loadResources();
```

You may also have to execute all your initialization code after that, according to what type of scene you are loading. Probably we will soon make an example of a simple game with different levels.
