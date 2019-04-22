package com.wengzc.okhttpnative;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.content.pm.PermissionGroupInfo;
import android.content.pm.PermissionInfo;
import android.os.Build;
import android.text.TextUtils;

import java.util.ArrayList;
import java.util.List;

public class PermissionUtil {


    public static boolean needRequestPermissions (Activity context, String ... permissionNeed){
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            List<String> permissionUngranted = collectUngrantedPermissions(context, permissionNeed);

            if (permissionUngranted.size() > 0) {
                return true;
            }
        }
        return false;
    }

    public static void requestPermissions (Activity context, int permissionReqCode, String ... permissionNeed){
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            List<String> permissionUngranted = collectUngrantedPermissions(context, permissionNeed);

            if (permissionUngranted.size() > 0) {
                context.requestPermissions(permissionUngranted.toArray(new String[]{}), permissionReqCode);
            }
        }
    }

    @TargetApi(Build.VERSION_CODES.M)
    public static void showRequestPermissionsResult (Activity context, String[] permissions, int[] grantResults){
        if (grantResults.length > 0) {
            List<String> deniedPermissionList = new ArrayList<>();
            for (int i = 0; i < grantResults.length; i++) {
                if (grantResults[i] != PackageManager.PERMISSION_GRANTED) {
                    deniedPermissionList.add(permissions[i]);
                }
            }

            if (!deniedPermissionList.isEmpty()) {
                String permissionHintMsg = permissionHintMsg(context, deniedPermissionList);

                if (!TextUtils.isEmpty(permissionHintMsg)){
                    //展示提示信息
                }
            }
        }
    }

    @TargetApi(Build.VERSION_CODES.M)
    private static  List<String> collectUngrantedPermissions (Activity context, String ... permissionNeed){
        List<String> permissionsUngranted = new ArrayList<>();

        if (permissionNeed != null){
            for (String p : permissionNeed){
                if (context.checkSelfPermission(p) != PackageManager.PERMISSION_GRANTED) {
                    permissionsUngranted.add(p);
                }
            }
        }
        return permissionsUngranted;
    }

    @TargetApi(Build.VERSION_CODES.M)
    private static String permissionHintMsg (Activity context, List<String> deniedPermissionList){
        StringBuilder stringBuilder = new StringBuilder();
        for (String deniedPermission : deniedPermissionList) {
            //勾选了对话框中”Don’t ask again”的选项, 返回false
            boolean flag = context.shouldShowRequestPermissionRationale(deniedPermission);
            if (flag) {
                continue;
            }

            try {
                PermissionInfo permissionInfo = context.getPackageManager().getPermissionInfo(deniedPermission, 0);
                PermissionGroupInfo permissionGroupInfo = context.getPackageManager().getPermissionGroupInfo(permissionInfo.group, 0);
                String permissionGroupName = permissionGroupInfo.loadLabel(context.getPackageManager()).toString();
                if (stringBuilder.indexOf(permissionGroupName) > 0){
                    continue;
                }
                stringBuilder.append("\n");
                stringBuilder.append(permissionGroupName);
            } catch (PackageManager.NameNotFoundException e) {
                e.printStackTrace();
            }
        }
        return stringBuilder.toString();
    }

    private PermissionUtil(){}

}
