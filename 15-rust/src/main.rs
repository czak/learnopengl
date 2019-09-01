extern crate gl;
extern crate sdl2;

use sdl2::event::Event;
use sdl2::keyboard::Keycode;

fn main() {
    let sdl_context = sdl2::init().unwrap();
    let video_subsystem = sdl_context.video().unwrap();

    let window = video_subsystem
        .window("rust opengl example", 800, 600)
        .position_centered()
        .opengl()
        .build()
        .unwrap();

    let gl_attr = video_subsystem.gl_attr();
    gl_attr.set_context_version(4, 5);
    gl_attr.set_context_profile(sdl2::video::GLProfile::Core);

    let _gl_context = window.gl_create_context().unwrap();
    gl::load_with(|s| video_subsystem.gl_get_proc_address(s) as *const std::os::raw::c_void);

    let mut event_pump = sdl_context.event_pump().unwrap();

    // Init
    let vao = unsafe {
        gl::ClearColor(0.3, 0.0, 0.5, 1.0);

        gl::Viewport(0, 0, 800, 600);

        let vertices: [f32; 12] = [
            0.5, 0.5, 0.0, 0.5, -0.5, 0.0, -0.5, -0.5, 0.0, -0.5, 0.5, 0.0,
        ];

        let mut vao = 0;
        gl::GenVertexArrays(1, &mut vao);
        gl::BindVertexArray(vao);

        let mut vbo = 0;
        gl::GenBuffers(1, &mut vbo);
        gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
        gl::BufferData(
            gl::ARRAY_BUFFER,
            (vertices.len() * std::mem::size_of::<f32>()) as gl::types::GLsizeiptr,
            vertices.as_ptr() as *const gl::types::GLvoid,
            gl::STATIC_DRAW,
        );

        gl::VertexAttribPointer(
            0,
            3,
            gl::FLOAT,
            gl::FALSE,
            (3 * std::mem::size_of::<f32>()) as gl::types::GLint,
            std::ptr::null(),
        );
        gl::EnableVertexAttribArray(0);

        let vertex_shader = load_shader("shaders/vert.glsl", gl::VERTEX_SHADER);
        let fragment_shader = load_shader("shaders/frag.glsl", gl::FRAGMENT_SHADER);

        let mut pipeline = 0;
        gl::GenProgramPipelines(1, &mut pipeline);
        gl::UseProgramStages(pipeline, gl::VERTEX_SHADER_BIT, vertex_shader);
        gl::UseProgramStages(pipeline, gl::FRAGMENT_SHADER_BIT, fragment_shader);
        gl::BindProgramPipeline(pipeline);

        vao
    };

    'running: loop {
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit { .. }
                | Event::KeyDown {
                    keycode: Some(Keycode::Escape),
                    ..
                } => break 'running,
                _ => {}
            }
        }

        unsafe {
            gl::Clear(gl::COLOR_BUFFER_BIT);

            gl::BindVertexArray(vao);
            gl::DrawArrays(gl::TRIANGLES, 0, 3);
            gl::BindVertexArray(0);
        }

        window.gl_swap_window();
    }
}

fn wsc(len: usize) -> std::ffi::CString {
    let mut buffer: Vec<u8> = Vec::with_capacity(len + 1);
    buffer.extend([b' '].iter().cycle().take(len));
    unsafe { std::ffi::CString::from_vec_unchecked(buffer) }
}

fn load_shader(filename: &str, kind: gl::types::GLuint) -> gl::types::GLuint {
    let source = std::fs::read_to_string(filename).unwrap();
    let c_source = std::ffi::CString::new(source).unwrap();
    unsafe {
        let shader = gl::CreateShaderProgramv(kind, 1, &c_source.as_ptr());

        let mut success = 0;
        gl::GetProgramiv(shader, gl::LINK_STATUS, &mut success);
        if success == 0 {
            let error = wsc(512);
            gl::GetProgramInfoLog(shader, 512, std::ptr::null_mut(), error.as_ptr() as *mut gl::types::GLchar);
            dbg!(error);
        }
        dbg!(success);

        shader
    }
}
