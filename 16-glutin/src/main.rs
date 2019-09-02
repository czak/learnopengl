extern crate gl;
extern crate glutin;

fn main() {
    let mut events_loop = glutin::EventsLoop::new();
    let window = glutin::WindowBuilder::new()
        .with_dimensions(glutin::dpi::LogicalSize::new(800.0, 600.0))
        .with_resizable(false);
    let gl_window = glutin::ContextBuilder::new()
        .build_windowed(window, &events_loop)
        .unwrap();

    let gl_window = unsafe { gl_window.make_current().unwrap() };

    gl::load_with(|s| gl_window.get_proc_address(s) as *const std::os::raw::c_void);

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

    events_loop.run_forever(|event| {
        use glutin::{ControlFlow, Event, WindowEvent};

        if let Event::WindowEvent { event, .. } = event {
            if let WindowEvent::CloseRequested = event {
                return ControlFlow::Break;
            }
        }

        unsafe {
            gl::Clear(gl::COLOR_BUFFER_BIT);

            gl::BindVertexArray(vao);
            gl::DrawArrays(gl::TRIANGLES, 0, 3);
            gl::BindVertexArray(0);
        }

        gl_window.swap_buffers().unwrap();

        ControlFlow::Continue
    });
}

fn load_shader(filename: &str, kind: gl::types::GLuint) -> gl::types::GLuint {
    let source = std::fs::read_to_string(filename).unwrap();
    let c_source = std::ffi::CString::new(source).unwrap();
    unsafe {
        let shader = gl::CreateShaderProgramv(kind, 1, &c_source.as_ptr());

        let mut success = 0;
        gl::GetProgramiv(shader, gl::LINK_STATUS, &mut success);
        if success == 0 {
            let mut buffer: Vec<u8> = Vec::with_capacity(512);
            let mut len = 0;
            gl::GetProgramInfoLog(shader, 512, &mut len, buffer.as_ptr() as *mut i8);
            buffer.set_len(len as usize);
            dbg!(String::from_utf8(buffer).unwrap());
        }

        shader
    }
}
