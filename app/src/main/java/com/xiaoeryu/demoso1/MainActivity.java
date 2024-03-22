package com.xiaoeryu.demoso1;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.xiaoeryu.demoso1.databinding.ActivityMainBinding;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'demoso1' library on application startup.
    static {
        System.loadLibrary("demoso1");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
//        init();
        while (true){
            try {
                Thread.sleep(3000);
            } catch (InterruptedException e) {
//                throw new RuntimeException(e);
                e.printStackTrace();
            }
//            Log.i("r0add", MainActivity.stringFromJNI());
//            Log.i("r0add", MainActivity.stringFromJNI2());
//            Log.i("r0add", MainActivity.myfirstjniJNI("xiaoeryu"));
//            Log.i("r0add", String.valueOf(this.myfirstjni()));
        }
    }
    public void testField() throws ClassNotFoundException, NoSuchFieldException, IllegalAccessException {
        Class testClazz = null;
        testClazz = MainActivity.class.getClassLoader().loadClass("com.xiaoeryu.demoso1.Test");
        Log.i("r0reflection", "Classloader.loadClass->" + testClazz);
        Class testClazz2 = null;
        testClazz2 = Class.forName("com.xiaoeryu.demoso1.Test");
        Log.i("r0reflection", "Class.forName->" + testClazz2);
        Class testClazz3 = Test.class;
        Log.i("r0reflection", ".class->" + testClazz3.getName());
        Field publicStaticField_field = testClazz3.getDeclaredField("publicStaticField");
        Log.i("r0reflection", "testClazz3.getDeclaredField->" + publicStaticField_field);
        String value = (String) publicStaticField_field.get(null);
        Log.i("r0reflection", "publicStaticField_field.get->" + value);

        Field privateStaticField_field = testClazz3.getDeclaredField("privateStaticField");
        privateStaticField_field.setAccessible(true);
        privateStaticField_field.set(null,"modified");
        String valuePrivate =  (String) privateStaticField_field.get(null);
        Log.i("r0reflection", "privateStaticField_field.get->" + valuePrivate);

        Field[] fields = testClazz3.getDeclaredFields();
        for (Field field : fields) {
            Log.i("r0reflection", "testClazz3.getDeclaredFields->" + field.getName());
        }
        Field[] fields2 = testClazz3.getFields();
        for (Field field : fields2) {
            Log.i("r0reflection", "testClazz3.getFields->" + field.getName());
        }
    }

    public void testMethod() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        Class testClazz = Test.class;
        Method publicStaticFunc_method = null;
        publicStaticFunc_method = testClazz.getDeclaredMethod("publicStaticFunc");
        Log.i("r0reflection", "testClazz.getDeclaredMethod->" + publicStaticFunc_method);
        Method privateStaticFunc_method = null;
        privateStaticFunc_method = testClazz.getDeclaredMethod("privateStaticFunc");
        Log.i("r0reflection", "testClazz.getDeclaredMethod->" + privateStaticFunc_method);
        privateStaticFunc_method.setAccessible(true);
        privateStaticFunc_method.invoke(null);
        Method[] methods = testClazz.getMethods();
        for (Method method : methods) {
            Log.i("r0reflection", "testClazz.getMethods->" + method.getName());
        }
        Method[] methods2 = testClazz.getDeclaredMethods();
        for (Method method : methods2) {
            Log.i("r0reflection", "testClazz.getDeclaredMethods->" + method.getName());
        }
    }

    /**
     * A native method that is implemented by the 'demoso1' native library,
     * which is packaged with this application.
     */
    public static native String stringFromJNI();
    public static native String stringFromJNI2();
    public static native String myfirstjniJNI(String context);
    public static native int myfirstjni();
    public native int init();
}