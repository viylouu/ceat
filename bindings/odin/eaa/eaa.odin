package ear

import "core:c"

import "../eau"

import ma "vendor:miniaudio"

when ODIN_OS == .Windows {
    foreign import ceat {
        "../../../build/ceat.lib",
    }
} else when ODIN_OS == .Linux {
    foreign import ceat {
        "../../../build/libceat.a",
    }
} else {
    // unsupported os!
}

_sound_ll :: struct{
    sound: ^_sound,
    next, prev: ^_sound_ll,
}

_sound_desc :: struct{
    volume: f32,
    pitch: f32,
    loop: bool,

    mixer: ^_mixer
}

_sound :: struct{
    ll: _sound_ll,

    ma_sound: ma.sound,
    ma_decode: ma.decoder,

    desc: _sound_desc,

    dest: ^eau.Destructor,
}

Sound :: struct{
    using sound: ^_sound,

    delete: proc(sound: ^Sound),
    play: proc(sound: ^Sound),
    stop: proc(sound: ^Sound),
    set_volume: proc(sound: ^Sound, volume: f32),
    set_pitch: proc(sound: ^Sound, pitch: f32),
    set_loop: proc(sound: ^Sound, loop: bool),
}

SoundDesc :: struct{
    volume, pitch: f32,
    loop: bool,
    mixer: ^Mixer,
}

_mixer_desc :: struct{
    volume: f32,
    pitch: f32,
    parent: ^_mixer,
}

_mixer :: struct{
    desc: _mixer_desc,
    dest: ^eau.Destructor,
}

Mixer :: struct{
    using mixer: ^_mixer,

    delete: proc(mixer: ^Mixer),
    set_volume: proc(mixer: ^Mixer, volume: f32),
    set_pitch: proc(mixer: ^Mixer, pitch: f32),
}

MixerDesc :: struct{
    volume, pitch: f32,
    parent: ^Mixer,
}

@(default_calling_convention="c")
foreign ceat {
    @(link_name="eaa_load_sound") _load_sound :: proc(desc: _sound_desc, data: ^u8, data_size: u32, arena: ^eau._arena) -> ^_sound ---
    @(link_name="eaa_delete_sound") _delete_sound :: proc(sound: ^_sound) ---
    @(link_name="eaa_play_sound") _play_sound :: proc(sound: ^_sound) ---
    @(link_name="eaa_stop_sound") _stop_sound :: proc(sound: ^_sound) ---
    @(link_name="eaa_set_sound_volume") _set_sound_volume :: proc(sound: ^_sound, volume: f32) ---
    @(link_name="eaa_set_sound_pitch") _set_sound_pitch :: proc(sound: ^_sound, pitch: f32) ---
    @(link_name="eaa_set_sound_loop") _set_sound_loop :: proc(sound: ^_sound, loop: bool) ---


    @(link_name="eaa_create_mixer") _create_mixer :: proc(desc: _mixer_desc, arena: ^eau._arena) -> ^_mixer ---
    @(link_name="eaa_delete_mixer") _delete_mixer :: proc(mixer: ^_mixer) ---
    @(link_name="eaa_set_mixer_volume") _set_mixer_volume :: proc(mixer: ^_mixer, volume: f32) ---
    @(link_name="eaa_set_mixer_pitch") _set_mixer_pitch :: proc(mixer: ^_mixer, pitch: f32) ---
}

load_sound :: proc(desc: SoundDesc, data: []u8, arena: ^eau.Arena = nil) -> ^Sound {
    return new_clone(Sound{
        sound = _load_sound(
            _sound_desc{
                volume = desc.volume,
                pitch = desc.pitch,
                loop = desc.loop,
                mixer = desc.mixer == nil? nil : desc.mixer.mixer,
                },
            &data[0],
            u32(len(data)),
            arena == nil? nil : arena.arena,
            ),

        delete = delete_sound,
        play = play_sound,
        stop = stop_sound,
        set_volume = set_sound_volume,
        set_pitch = set_sound_pitch,
        set_loop = set_sound_loop,
        })
}

delete_sound :: proc(sound: ^Sound) {
    _delete_sound(sound.sound)
    free(sound)
}

play_sound :: proc(sound: ^Sound) {
    _play_sound(sound.sound)
}

stop_sound :: proc(sound: ^Sound) {
    _stop_sound(sound.sound)
}

set_sound_volume :: proc(sound: ^Sound, volume: f32) {
    _set_sound_volume(sound.sound, volume)
}

set_sound_pitch :: proc(sound: ^Sound, pitch: f32) {
    _set_sound_pitch(sound.sound, pitch)
}

set_sound_loop :: proc(sound: ^Sound, loop: bool) {
    _set_sound_loop(sound.sound, loop)
}


create_mixer :: proc(desc: MixerDesc, arena: ^eau.Arena = nil) -> ^Mixer {
    return new_clone(Mixer{
        mixer = _create_mixer(
            _mixer_desc{
                volume = desc.volume,
                pitch = desc.pitch,
                parent = desc.parent == nil? nil : desc.parent.mixer,
                },
            arena == nil? nil : arena.arena,
            ),

        delete = delete_mixer,
        set_volume = set_mixer_volume,
        set_pitch = set_mixer_pitch,
        })
}

delete_mixer :: proc(mixer: ^Mixer) {
    _delete_mixer(mixer.mixer)
    free(mixer)
}

set_mixer_volume :: proc(mixer: ^Mixer, volume: f32) {
    _set_mixer_volume(mixer.mixer, volume)
}

set_mixer_pitch :: proc(mixer: ^Mixer, pitch: f32) {
    _set_mixer_pitch(mixer.mixer, pitch)
}
